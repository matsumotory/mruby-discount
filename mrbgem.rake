MRuby::Gem::Specification.new('mruby-markdown') do |spec|
  spec.license = 'MIT'
  spec.authors = 'MATSUMOTO Ryosuke'
  spec.linker.libraries << 'markdown'
end
