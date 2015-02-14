MRuby::Gem::Specification.new('mruby-discount') do |spec|
  spec.license = 'MIT'
  spec.authors = 'MATSUMOTO Ryosuke'
  spec.linker.libraries << 'markdown'
  require 'open3'

  discount_dir = "#{build_dir}/discount"

  def run_command env, command
    STDOUT.sync = true
    puts "build: [exec] #{command}"
    Open3.popen2e(env, command) do |stdin, stdout, thread|
      print stdout.read
      fail "#{command} failed" if thread.value != 0
    end
  end

  FileUtils.mkdir_p build_dir

  if ! File.exists? discount_dir
    Dir.chdir(build_dir) do
      e = {}
      run_command e, 'git clone git://github.com/Orc/discount.git'
    end
  end

  if ! File.exists? "#{discount_dir}/libmarkdown.a"
    Dir.chdir discount_dir do
      e = {}
      run_command e, './configure.sh'
      run_command e, 'make'
    end
  end

  spec.cc.include_paths << discount_dir
  spec.linker.library_paths << discount_dir
end
