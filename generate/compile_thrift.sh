#!/bin/sh

if [ $# -ne 2 ]
then
  echo "usage: `basename $0` <proj_root_src_dir> <output_makefile>"
  exit 1
fi

proj_root_src_dir=`readlink -f $1`
output_makefile=$2
entity_dir=${proj_root_src_dir}/entity/meta
global_dir=${proj_root_src_dir}/global/meta
py_output_dir=${proj_root_src_dir}/../generate/configure/extension/python

if [ ! -d $proj_root_src_dir -o ! -d $entity_dir -o ! -d $global_dir ]
then
  echo "$proj_root_src_dir is not a src directroy"
  exit 1
fi

cpp_thrift_args="--gen cpp -I $entity_dir -I $global_dir"
py_thrift_args="--gen py -I $entity_dir -I $global_dir -out $py_output_dir"
make_dep_file_list=''
cpp_make_rules=''

for thrift_file in `find $proj_root_src_dir -type f -name '*.thrift'`
do
  # generate cpp_make_rules
  thrift_file_dir=`dirname $thrift_file`
  thrift_file_header="${thrift_file_dir}/../`basename $thrift_file .thrift`_types.h"
  make_dep_file_list="$make_dep_file_list $thrift_file_header"
  dep_file_list=`grep '^include' $thrift_file | sed -e 's/^.*include *"\(.*\)".*$/\1/g'`
  abs_path_dep_file_list=''

  for dep_file in $dep_file_list
  do
    if [ -f ${thrift_file_dir}/${dep_file} ]
    then
      abs_path_dep_file_list="${abs_path_dep_file_list} ${thrift_file_dir}/${dep_file}"
    elif [ -f ${entity_dir}/${dep_file} ]
    then
      abs_path_dep_file_list="${abs_path_dep_file_list} ${entity_dir}/${dep_file}"
    elif [ -f ${global_dir}/${dep_file} ]
    then
      abs_path_dep_file_list="${abs_path_dep_file_list} ${global_dir}/${dep_file}"
    fi
  done

  cpp_make_rules="${cpp_make_rules}\n\
$thrift_file_header : $thrift_file $abs_path_dep_file_list
\tthrift $cpp_thrift_args -out $thrift_file_dir/.. $thrift_file\n"

  # generate py_make_rules
  py_namespace=`grep '^namespace py' $thrift_file | sed 's/^.*namespace py \(.*\)$/\1/g'`
  if [ ! -z ${py_namespace} ]
  then
    py_file=${py_output_dir}/`echo ${py_namespace} | sed 's,\.,/,g'`/ttypes.py
    make_dep_file_list="$make_dep_file_list $py_file"

    py_make_rules="${py_make_rules}\n\
$py_file : $thrift_file $abs_path_dep_file_list
\tthrift $py_thrift_args $thrift_file\n"
  fi
done


make_dep_file_list="$make_dep_file_list py_init_clear"

py_init_clear_rules="\npy_init_clear :
\tfind $py_output_dir/ -iname __init__.py -exec cp /dev/null {} \\;\n"

printf "\
.PHONY: all

all: $make_dep_file_list
\t
" >$output_makefile

printf "$cpp_make_rules" >>$output_makefile
printf "$py_make_rules" >>$output_makefile
printf "$py_init_clear_rules" >>$output_makefile

exit 0
