#! .env/bin/python

from operator import attrgetter
from typing import Union
import yaml
import bs4
import os
import requests
import click
import re
import time
from collections import namedtuple
import logging

logging.basicConfig(level=logging.DEBUG)

Solution = namedtuple("Solution", "id, name, status, source, lang")


class Parser:
    def __init__(self, username , password, contest_id, 
                 save_to_folder="./content", 
                 ejudge_url="https://ejudge.atp-fivt.org"):
        self.session: requests.Session = requests.Session()

        self.ejudge_url = ejudge_url
        self.username = username    
        self.password = password
        self.contest_id = contest_id
        self.sleep_time=1
        self.create_folders = True
        self.save_to_folder = save_to_folder

        self.formats = self.prep_formats("./lang_formats.yml")
        self.logger = logging.getLogger("ejudge_parser")

    def prep_formats(self, path):
        try:
            with open(path, "r") as f:
                return yaml.load(f, Loader=yaml.FullLoader)
        except IOError as e:
            self.logger.error("io error", exc_info=True)
            return {}
        except yaml.scanner.ScannerError as e:
            self.logger.error("wrong yaml format", exc_info=True)
            return {}

    def login(self):
        # Fill in your details here to be posted to the login form.
        payload = {
            'contest_id': self.contest_id,
            'role': '0',
            'login': self.username,
            'password': self.password,
            "locale_id": 0,
            "action_2": "Log in",
        }

        assert len(payload['login']) !=0 and len(payload['password']) !=0 , "No login info"
        assert len(payload['contest_id']) != 0, "No contest id"

        p = self.session.post(f'{self.ejudge_url}/client', data=payload)
        soup: bs4.BeautifulSoup = bs4.BeautifulSoup(p.content,features="html.parser")

        links: bs4.ResultSet = soup.find_all("a", href=True)

        assert len(links) > 0

        link: bs4.Tag = links[0]

        link_text: str = link.attrs['href']

        sid_list: list = re.findall("SID=(.*)&", link_text)

        assert len(sid_list) == 1, "No session token"
        self.sid: str = sid_list[0]

    @staticmethod
    def find_td_source(row: bs4.ResultSet) -> Solution:
        parsed_row = row.findAll("td")
        id: int = int(parsed_row[0].text)
        name: str = parsed_row[3].text
        lang: str = parsed_row[4].text
        status: str = parsed_row[5].text
        source: str = parsed_row[8].find('a').attrs['href']

        return Solution(id, name, status, source, lang)

    def convert_table_to_list(self, rows: bs4.ResultSet) -> list:
        solution_list: list = []

        for row in rows:
            solution: Solution = self.find_td_source(row)
            solution_list.append(solution)

        solution_list = sorted(solution_list, key=attrgetter('id'), reverse=True)

        return solution_list

    def sort_by_status(self, problem_list: list, status: str = "OK"):
        filtered_list = list(filter(lambda x: x.status == status, problem_list))
        return filtered_list

    def get_problem_name(self, problem_container):
        problem_name_container: Union[bs4.BeautifulSoup, bs4.NavigableString] = problem_container.find("h3")
        problem_name: str = problem_name_container.text.split("Problem ")[1]
        return problem_name

    def find_code(self, soup):
        c = soup.find("pre", class_="line-numbers")
        code = c.find('code')
        return code.text

    def download_solution(self, solution):
        if self.create_folders:
            prob_path = f"{self.save_to_folder}/{solution.name}"
        else:
            prob_path = f"{self.save_to_folder}"

        if self.create_folders:
            try:
                os.makedirs(f"{prob_path}", exist_ok=True)
            except OSError:
                self.logger.error(f"failed to create path={prob_path}")
            else:
                self.logger.debug(f"path {prob_path} created successfully")

        r = self.session.get(solution.source)
        soup = bs4.BeautifulSoup(r.content, features="html.parser")
        code = self.find_code(soup)

        lang = self.formats.get(solution.lang)
        if self.formats and lang:
            format = lang
        else:
            format = "txt"

        with open(f"{prob_path}/{solution.name}.{format}", "w") as f:
            f.write(code)

        self.logger.info(f"problem {solution.name} saved")

    def parse_problem(self, problem_id: int) -> None:
        url = f"{self.ejudge_url}/client?SID={self.sid}&action=139&prob_id={problem_id}"
        r = self.session.get(url)
        soup: bs4.BeautifulSoup = bs4.BeautifulSoup(r.content,features="html.parser")

        problem_container: Union[bs4.BeautifulSoup, bs4.NavigableString] = soup.find("table", class_="probNav")
        table: Union[bs4.Tag, bs4.NavigableString, int] = problem_container.find("table", class_="table")
        if table is None:
            self.logger.error(f"parsing error at id = {problem_id}")
            return
        table_rows: bs4.ResultSet = table.findAll("tr")
        problem_list = self.convert_table_to_list(table_rows[1:])
        if len(problem_list) == 0:
            self.logger.error("solution list empty")
            return

        filtered_problems = self.sort_by_status(problem_list)
        if len(filtered_problems) == 0:
            self.logger.error("didn't find any correct solutions")
            return

        solution: Solution = filtered_problems[0]
        self.download_solution(solution)

    def run(self, count):
        self.login()
        for i in range(1, 1+count):
            self.parse_problem(i)
            time.sleep(self.sleep_time)


@click.command()
@click.option('--count', prompt="Problem count",
                         default=1,
                         help='Number of problems the script should download (starting with id=1).')
@click.option('--username', prompt='Ejudge username', 
                            help='This script uses your username/pass to access the ejudge interface.')
@click.option('--password', prompt='Ejudge password', 
                            help='This script uses your username/pass to access the ejudge interface.')
@click.option('--contest_id', prompt='Ejudge contest id', 
                            help='Contest id to use (you can find it in your ejudge login interface url with the url key `contest_id`)')
@click.option('--problem_id', default=0, 
                              help='Problem id to download, (--count ignored if true)')
@click.option('--save_folder', default="./content/", 
                               help='path to save to problems to')
@click.option('--ejudge_url', default="https://ejudge.atp-fivt.org", 
                               help='ejudge url')
def cli(username, password, contest_id, count, problem_id, save_folder, ejudge_url):
    parser = Parser(username, password, contest_id, 
                    save_to_folder=save_folder, 
                    ejudge_url=ejudge_url)
    if problem_id == 0:
        parser.run(count)
    else:
        parser.parse_problem(problem_id)

if __name__ == '__main__':
    cli()


