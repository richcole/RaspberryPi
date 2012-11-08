class String
  def /(other)
    File.join(self, other)
  end
end

class Builder
  include Rake::DSL

  attr_accessor :build, :tests, :clean, :binary, :default

  def initialize(module_name)
    @proj_dir  = FileUtils.pwd
    @build_dir = @proj_dir / "build"

    @arm_asflags   = "--warn --fatal-warnings -mcpu=arm1176jzf-s -march=armv6"
    @arm_cflags    = "-O2 -Wall -nostdlib -nostartfiles -ffreestanding"
    @arm_ldflags   = "-Wall -m32"

    @arm_arch     = "arm-linux-gnueabi"
    # @arm_arch       = "arm-none-eabi"
    @arm_objcopy    = @arm_arch + "-objcopy"
    @arm_compiler   = @arm_arch + "-gcc"
    @arm_linker     = @arm_arch + "-ld"
    @arm_assembler  = @arm_arch + "-as"

    @native_cflags  = "-Wall -ggdb"
    @native_asflags = ""
    @native_ldflags = ""

    @native_objcopy    = "objcopy"
    @native_compiler   = "gcc"
    @native_linker     = "ld"

    @module_name    = module_name
    @build_dir      = @build_dir / @module_name
    @proj_dir       = @proj_dir  / @module_name
    @src_dir        = @proj_dir  / "src"
    @test_dir       = @proj_dir  / "test"

    @include_dir    = @proj_dir  / "include"
    @lib_dir        = @proj_dir  / "lib"
    @libs           = ""

    @arm_obj_dir    = @build_dir / "obj" / "arm"
    @arm_bin_dir    = @build_dir / "bin" / "arm"
    @arm_objs       = []

    @native_obj_dir = @build_dir / "obj" / "native"
    @native_bin_dir = @build_dir / "bin" / "native"
    @native_objs    = []

    directory @arm_bin_dir
    directory @arm_obj_dir
    directory @native_bin_dir
    directory @native_obj_dir

    @tests          = []
  end

  def tasks
    directory @native_obj_dir
    directory @native_bin_dir
    directory @arm_bin_dir
    directory @native_bin_dir

    @default = []
    @arm_objs = []
    Dir.glob(@src_dir / "*.s").each do |src_file| 
      @arm_objs += arm_assemble(src_file)
    end
    each_file(@src_dir, "*.cpp", "*.c") do |src_file|
      @arm_objs += arm_compile(src_file)
    end

    @native_objs = []
    each_file(@src_dir, "*.c") do |src_file|
      @native_objs += native_compile(src_file)
    end

    @tests = []
    @test_objs = []
    each_file(@test_dir, "*.c") do |src_file|
      @test_objs += native_compile(src_file)
    end
    Dir.glob(@test_dir / "test_*.c").each do |src_file|
      test_name = File.basename(src_file).sub(/\..*$/, '')
      objs = native_compile(src_file)
      binary = native_link(test_name, @native_objs + objs)
      @tests += execute(test_name, binary)
    end

    @clean = @module_name + "_clean"
    task @clean do
      sh "rm -rf #{build_dir}"
    end
  end

  def execute(name, binary)
    target = name + "_run"
    task target => binary do
      sh binary
    end
    return [ target ]
  end

  def each_file(base, *pat, &block) 
    pat.each do |p|
      Dir.glob(base / p, &block)
    end
  end

  def arm_compile(src_file)
    obj = @arm_obj_dir / File.basename(src_file) + ".o"
    file obj => [@arm_obj_dir, src_file] do
      sh "#{@arm_compiler} #{@arm_cflags} -I#{@include_dir} -I#{@src_dir} -c -o #{obj} #{src_file}"
    end
    return [ obj ]
  end

  def arm_assemble(src_file)
    obj = @arm_obj_dir / File.basename(src_file) + ".o"
    file obj => [@arm_obj_dir, src_file] do
      sh "#{@arm_assembler} #{@arm_asflags} -I{@include_dir} -I#{@src_dir} -o #{obj} #{src_file}"
    end
    return [ obj ]
  end

  def native_compile(src_file)
    obj = @native_obj_dir / File.basename(src_file) + ".o"
    file obj => [@native_obj_dir, src_file] do
      sh "#{@native_compiler} #{@native_cflags} -I#{@src_dir} -c -o #{obj} #{src_file}"
    end
    return [ obj ]
  end

  def native_link(test_name, obj_files)
    test_bin = @native_bin_dir / test_name
    file test_bin => [ @native_bin_dir ] + obj_files do
      sh "#{@native_compiler} -o #{test_bin} #{obj_files.join(" ")}"
    end
    return test_bin
  end

end

class KernelBuilder < Builder

  attr_accessor :arch, :elf_binary, :binary

  def initialize(module_name)
    super(module_name)
    @binary       = @build_dir / "kernel.bin"
    @elf_binary   = @build_dir / "kernel.elf"
    @libs         = ""
  end

  def tasks
    super()
    file @elf_binary => [@arm_bin_dir] + @arm_objs do
      linker_file = @src_dir / "linker.ld"
      sh "mkdir -p #{File.dirname(elf_binary)}"
      sh "#{@arm_linker} -T #{linker_file} #{@arm_objs.join(" ")} -o #{@elf_binary} -L#{@lib_dir} #{@libs}"
    end
    file @binary => [ @elf_binary ] do
      sh "mkdir -p #{File.dirname(binary)}"
      sh "#{@arm_objcopy} #{@elf_binary} -O binary #{@binary}"
    end

    @default << @elf_binary << @binary
  end

end

class SerialBuilder < Builder

  def initialize
    super("serial")
  end

  def tasks
    super
    @binary = native_link("serial", @native_objs)
    @default << @binary
  end

end

k1 = KernelBuilder.new("helloworld")
k2 = KernelBuilder.new("kernel")
s = SerialBuilder.new
device = "/dev/ttyUSB0"
# device = "/dev/tty.SLAB_USBtoUART"

builders = [k1, k2, s]
builders.each { |x| x.tasks }

task :tests => builders.map { |x| x.tests }.flatten

task :default => builders.map { |x| x.default }.flatten 

task :bootstrap do
  sh "sudo adduser $USER dialout"
  sh "sudo apt-get install gcc-4.4-arm-linux-gnueabi"
end

task :run => :helloworld

task :helloworld => [k1.binary, s.binary] do
  sh "#{s.binary} bootload #{k1.binary} #{device}"
end

task :run_kernel => [k2.binary, s.binary] do
  sh "#{s.binary} bootload #{k2.binary} #{device}"
end

task :kernel => [k2.binary, s.binary] 
  
task :clean do
  sh "rm -rf build"
end
