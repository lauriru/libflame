#!/bin/bash

main() 
{
    script_name=${0##*/}
    echo " "
    echo " "
    echo " "${script_name}
    echo " "
    echo " Objective:"
    echo "   The script removes comma expressions, which are automatically generated by abs macro."
    echo " "

    files="$(find . -maxdepth 1 -name "*.c")"
    for file in ${files}; do
        echo -ne "   Removing comma expressions from ... ${file}                  "\\r

        tmp_file=$(echo "${file}.back")
        ( mv -f ${file} ${tmp_file} ;
            cat ${tmp_file} \
                | sed  's/\([ a-z0-9_]*\) = \([][ &a-z0-9_/<>.,+*()-]*\), abs([ a-z0-9_]*)/(abs(\2))/g' \
                > ${file} ;
            rm -f ${tmp_file} )
    done

    return 0
}

main "$@"


# | sed  's/\([ a-z0-9_]*\) = \([a-z0-9_]*\)(\([][ &a-z0-9_.+*()-]*\)), \(abs\)([ a-z0-9_]*)/(kdd(\2(\3)))/g' 
