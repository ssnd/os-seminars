# Установка экспортера

- `python3 -m venv .env`
- `source .env/bin/activate`
- `pip install -r requirements.txt`


# Использование экспортера

```
Usage: ejudge_parser.py [OPTIONS]

Options:
  --count INTEGER       Number of problems the script should download
                        (starting with id=1).
  --username TEXT       This script uses your username/pass to access the
                        ejudge interface.
  --password TEXT       This script uses your username/pass to access the
                        ejudge interface.
  --contest_id TEXT     Contest id to use (you can find it in your ejudge
                        login interface url with the url key `contest_id`)
  --problem_id INTEGER  Problem id to download, (--count ignored if true)
  --save_folder TEXT    path to save to problems to (default `./content`)
  --ejudge_url TEXT     ejudge url (default `https://ejudge.atp-fivt.org`)
  --help                Show this message and exit.
```
