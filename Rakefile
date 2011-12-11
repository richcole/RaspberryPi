#!/usr/bin/rake

require 'fileutils'

class Builder
  attr_reader :proj_dir, :build_dir, :src_dir, :obj_dir
  attr_reader :tool_dir, :tool_lib, :qemu_dir
  attr_reader :arch, :cpu, :platform
  attr_reader :includes, :flags, :c_flags, :as_flags, :src_dirs
  attr_reader :cc, :as, :ld, :objcopy

  def initialize
    @arch      = "arm-none-eabi"
    @platform  = "qemu"
    @qemu_dir  = "/home/richcole/raspberry-pi/raspidev/bin"
    @tool_dir  = "/home/richcole/raspberry-pi/raspidev/arm-2011.03/bin"
    @tool_lib  = "/home/richcole/raspberry-pi/raspidev/arm-2011.03/lib/gcc/arm-none-eabi/4.5.2/libgcc.a"
    @proj_dir  = FileUtils.pwd
    @build_dir = proj_dir  + "/build"
    @src_dir   = proj_dir  + "/src"
    @obj_dir   = build_dir + "/obj"
    @as        = @tool_dir + "/" + arch + "-as"
    @cc        = @tool_dir + "/" + arch + "-gcc"
    @ld        = @tool_dir + "/" + arch + "-ld"
    @objcopy   = @tool_dir + "/" + arch + "-objcopy"
    @cpu       = "arm1176jzf-s"
    @src_dirs  = [src_dir, src_dir + "/" + platform, src_dir + "/lib", src_dir + "/lambda"]
    @includes  = @src_dirs.map { |x| "-I#{x}" }.join(" ")
    @flags     = "#{includes} -g -mcpu=#{cpu}"
    @c_flags   = "#{flags} -DPLATFORM=#{platform}"
    @as_flags  = "#{flags}"
  end

  def run_as(src_file)
    obj_file = obj_dir + "/" + File.basename(src_file).gsub(".s", ".o")
    file(obj_file => [ src_file, obj_dir, build_dir ]) do 
      run "#{as} #{as_flags} -o #{obj_file} #{src_file}"
    end
    obj_file
  end

  def run_cc(src_file)
    obj_file = obj_dir + "/" + File.basename(src_file).gsub(".c", ".o")
    file(obj_file => [ src_file, obj_dir, build_dir ]) do 
      run "#{cc} -c #{c_flags} -O0 -Werror -std=c99 -mcpu=#{cpu} " + 
        "-o #{obj_file} #{src_file}"
    end
    obj_file
  end

  def build_kernel_elf_image(ld_file, obj_files, kernel_file)
    kernel_elf_image = kernel_file + ".elf"
    deps = [ld_file, build_dir] + obj_files
    file(kernel_elf_image => deps) do
      ld_flags = "-nostdlib -static --error-unresolved-symbols"
      run "#{ld} #{ld_flags} -T #{ld_file} #{obj_files.join(" ")} " + 
        "-o #{kernel_elf_image} #{tool_lib}"
      run "#{objcopy} -O binary #{kernel_elf_image} #{kernel_image}"
    end
    kernel_elf_image
  end

  def build_kernel_image(kernel_file, kernel_elf_image)
    kernel_image = kernel_file + ".img"
    file(kernel_image => kernel_elf_image) do
      run "#{objcopy} -O binary #{kernel_elf_image} #{kernel_image}"
    end
    kernel_image
  end

  def tasks
    directory build_dir 
    directory obj_dir

    task :clean do
      clean
    end

    obj_files = []
    for sdir in src_dirs do
      for sfile in Dir.glob("#{sdir}/*.s") do
        obj_files << run_as(sfile)
      end
      for sfile in Dir.glob("#{sdir}/*.c") do
        obj_files << run_cc(sfile)
      end
    end
    kernel_file = build_dir + "/kernel"
    kernel_elf_image = build_kernel_elf_image("#{src_dir}/kernel.ld", obj_files, kernel_file)
    kernel_image = build_kernel_image(kernel_file, kernel_elf_image)
    task :default => kernel_elf_image
    task :run => run_kernel(kernel_elf_image)
  end

  def run_kernel(kernel_image)
    task :run_kernel => [ kernel_image ] do
      run "#{qemu_dir}/qemu-system-arm -M versatilepb -cpu arm1136-r2 -m 128 -device sp804 -device pl011 -kernel #{kernel_image} -nographic"
    end
  end

  def clean
    run "rm -rf #{build_dir}"
  end

  def run(cmd)
    sh cmd
  end

end

builder = Builder.new

builder.tasks
