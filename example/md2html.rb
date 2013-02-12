m = Markdown.new("http://kevinburke.bitbucket.org/markdowncss/markdown.css", "titlehoge")

markdown = '
# title
title
## subtitle
- hoge
- fuga
'

puts m.header
puts m.md2html(markdown)
puts m.md2html("## subtitle2")
puts m.md2html("- foo")
puts m.footer
