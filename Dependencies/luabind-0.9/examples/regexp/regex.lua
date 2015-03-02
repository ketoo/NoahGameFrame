
html_tag = regex("<(.*?)>")
src = '<html>dadas<head>dsa</head></html>'

while html_tag:search(src) do
	print('tag: ' .. html_tag:what(1))
	src = string.sub(src, html_tag:position(0) + html_tag:length(0))
end

print('-------------')


