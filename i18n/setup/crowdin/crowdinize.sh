#!/usr/bin/env bash

shopt -s extglob

files=$@

for file in ${files[@]}; do
  output=
  template=

  if [ "$file" != "${file/%template*.xml/}" ]; then
    template=1
  fi

  while IFS= read -r line; do
    compare=

#   Append newline character
    if [ -n "$output" ]; then
      output=${output}$'\n'
    fi

    if [ "$template" == 1 ]; then
#     Duplicate string attribute in content
      if [ "$line" != "${line/<entry id=/}" ]; then
        string=${line##*string=\"}
        string=${string%%\"*}
        line=${line/%\/>/>$string<\/entry>}
      fi      
    else
#     Find comparison string in line
      if [ "$line" != "${line/<entry id=/}" ]; then
        compare=${line##*([[:space:]])}
        compare=${compare/%[[:space:]]string=*/}
      elif [ "$line" != "${line/<property name=/}" ]; then
        compare=${line##*([[:space:]])}
        compare=${compare/%>*/}
      fi

#     Compare to file and decide which line to use
      if [ -n "$compare" ]; then
        match=`grep -m 1 "$compare" "$file"`

        if [ -n "$match" ]; then
          line="$match"
        fi
      fi
    fi

    output=${output}${line}
  done < "template.xml"

  echo "$output" > "$file"
done
