# Markdown Class for mruby
mruby markdown class.

convert markdown to html using [discount](https://github.com/Orc/discount)

## install by mrbgems
 - add conf.gem line to `build_config.rb`
```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :git => 'https://github.com/matsumoto-r/mruby-discount.git'
end
```

## example

```ruby
m = Discount.new("http://kevinburke.bitbucket.org/markdowncss/markdown.css", "titlehoge")

markdown = '
# title
title
## subtitle
- hoge
- fuga
'

puts m.header
puts markdown.to_html
puts "## subtitle2".to_html
puts "- foo".to_html
puts m.footer

# # Or
# puts m.md2html(markdown + "\n" + "## subtitle2" + "- foo")
```


 ```html
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta http-equiv="Content-Style-Type" content="text/css" />
<title>titlehoge</title>
<link rel="stylesheet" href="http://kevinburke.bitbucket.org/markdowncss/markdown.css" type="text/css" />
</head>
<body>
<a name="title"></a>
<h1>title</h1>

<p>title</p>

<a name="subtitle"></a>
<h2>subtitle</h2>

<ul>
<li>hoge</li>
<li>fuga</li>
</ul>
<a name="subtitle2"></a>
<h2>subtitle2</h2>
<ul>
<li>foo</li>
</ul>

</body>
</html>
 ```

# License
under the MIT License:

* http://www.opensource.org/licenses/mit-license.php


