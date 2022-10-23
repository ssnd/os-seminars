.intel_syntax noprefix

.text
.global add

add:
	vaddsd	xmm0, xmm0, xmm1
	ret