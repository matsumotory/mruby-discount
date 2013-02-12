# Markdown Module for mruby
mruby markdown module.

convert markdown to html

## install by mrbgems
 - add conf.gem line to `build_config.rb`
```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :git => 'https://github.com/matsumoto-r/mruby-markdown.git'
end
```

## example

 - code

```ruby
m = Markdown.new("http://kevinburke.bitbucket.org/markdowncss/markdown.css", "titlename")

markdown = "
# title
title
## subtitle
- hoge
- fuga
"

puts m.header
puts m.md2html(markdown)
puts m.footer

```

 - result

 ```html
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html PUBLIC
              "-//W3C//DTD XHTML 1.0 Strict//EN"
              "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta http-equiv="Content-Style-Type" content="text/css" />
<link rel="stylesheet" href="http://kevinburke.bitbucket.org/markdowncss/markdown.css" type="text/css" />
<title>titlename</title>
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

</body>
</html>

 ```

# License
under the MIT License:

* http://www.opensource.org/licenses/mit-license.php


