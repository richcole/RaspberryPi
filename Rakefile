class String
  def /(other)
    File.join(self, other)
  end
end

class Builder
  include Rake::DSL

  attr_reader :proj_dir,  :build_dir, :src_dir, :obj_dir, :bin_dir
  attr_reader :assembler, :compiler,  :linker,  :objcopy
  attr_reader :cflags,    :ldflags,   :asflags
  attr_reader :objs,      :binary

  def initialize
    @proj_dir  = FileUtils.pwd
    @build_dir = proj_dir / "build"
    @cflags    = "-ggdb"
    @asflags   = ""
    @ldflags   = ""
  end

  def init_module(name)
    @build_dir      = @build_dir / name
    @proj_dir       = @proj_dir  / name
    @src_dir        = proj_dir  / "src"
    @obj_dir        = build_dir / "obj"
    @bin_dir        = build_dir / "bin"
    @objs           = []
  end

  def run_module
    directory obj_dir 
    Dir.glob(src_dir / "*.s").each do |src_file| 
      objs << assemble(src_file)
    end
    Dir.glob(src_dir / "*.cpp").each do |src_file| 
      objs << compile(src_file)
    end
    Dir.glob(src_dir / "*.c").each do |src_file| 
      objs << compile(src_file)
    end
  end

  def run
    task :clean do
      clean
    end
  end

  def clean
    sh "rm -rf #{build_dir}"
  end

  def default
    []
  end
  
  def compile(src_file)
    obj = obj_dir / File.basename(src_file) + ".o"
    task obj => [obj_dir, src_file] do
      sh "#{compiler} #{cflags} -I#{src_dir} -c -o #{obj} #{src_file}"
    end
  end

  def assemble(src_file)
    obj = obj_dir / File.basename(src_file) + ".o"
    task obj => [obj_dir, src_file] do
      sh "#{assembler} #{asflags} -I#{src_dir} -o #{obj} #{src_file}"
    end
  end

end

class KernelBuilder < Builder

  attr_accessor :arch, :elf_binary

  def initialize(module_name)
    super()
    init_module(module_name)
    @arch       = "arm-linux-gnueabi"
    @assembler  = arch + "-as"
    @compiler   = arch + "-gcc"
    @linker     = arch + "-ld"
    @objcopy    = arch + "-objcopy"
    @binary     = build_dir / "kernel.bin"
    @elf_binary = build_dir / "kernel.elf"

    @asflags   = "--warn --fatal-warnings -mcpu=arm1176jzf-s -march=armv6"
    @cflags    = "-Wall -nostdlib -nostartfiles -ffreestanding"
    @ldflags   = "-Wall -m32"
  end

  def default
    [binary]
  end

  def run
    run_module
    directory bin_dir
    task elf_binary => [bin_dir] + objs do
      linker_file = src_dir / "linker.ld"
      sh "mkdir -p #{File.dirname(elf_binary)}"
      sh "#{linker} -T #{linker_file} #{objs.join(" ")} -o #{elf_binary}"
    end
    task binary => [ elf_binary ] do
      sh "mkdir -p #{File.dirname(binary)}"
      sh "#{objcopy} #{elf_binary} -O binary #{binary}"
    end
  end

end

class SerialBuilder < Builder

  def initialize
    super
    init_module("serial")
    @binary         = bin_dir / "serial"
    @compiler       = "g++"
  end

  def run
    run_module
    directory bin_dir
    task binary => [ bin_dir ] + objs do
      sh "#{compiler} #{ldflags} -o #{binary} #{objs.join(" ")}"
    end
  end

  def default
    [binary]
  end

end

b = Builder.new
k1 = KernelBuilder.new("helloworld")
k2 = KernelBuilder.new("kernel")
s = SerialBuilder.new

builders = [b, k1, k2, s]

for x in builders do
  x.run
end

task :default => builders.map { |x| x.default }.flatten 

task :bootstrap do
  sh "sudo adduser $USER dialout"
  sh "sudo apt-get install gcc-4.4-arm-linux-gnueabi"
end

task :run => :helloworld

task :helloworld => [k1.binary, s.binary] do
  sh "#{s.binary} bootload #{k1.binary}"
end

task :run_kernel => [k2.binary, s.binary] do
  sh "#{s.binary} bootload #{k2.binary}"
end
  
task :kernel => [k2.binary, s.binary] 
  
task :clean do
  sh "rm -rf build"
end
