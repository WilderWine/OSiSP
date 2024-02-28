#!/bin/bash

zero=("ooo" "o o" "o o" "o o" "ooo")
one=("  o" " oo" "o o" "  o" "  o")
two=("ooo" "  o" "ooo" "o  " "ooo")
three=("ooo" "  o" "ooo" "  o" "ooo")
four=("o o" "o o" "ooo" "  o" "  o")
five=("ooo" "o  " "ooo" "  o" "ooo")
six=("ooo" "o  " "ooo" "o o" "ooo")
seven=("ooo" "  o" " o " " o " " o ")
eight=("ooo" "o o" "ooo" "o o" "ooo")
nine=("ooo" "o o" "ooo" "  o" "ooo")

print_number() {
    xxx=$1
    yyy=$2
    colour=$3
    local digit=("$@")
    count=0
    for element in "${digit[@]:3}"; do
	tput cup $(($xxx+$count)) $yyy
	echo -e "$colour$element\e[0m"
	count=$(($count+1))
    done

}

random() {
    seed=$(($(date +%s) * 9301 + 49297))
    rand=$(($seed % 233280))
    echo $rand
}

print_seconds_left(){
    vall=$1
    color=$2
    string_number="$vall"
    length=${#string_number}
    spacing=$(($length*4))
    rows=$(tput lines)
    columns=$(tput cols)
    xx=$(($(random) % ($rows - 9)))
    yy=$(($(random) % ($columns - $spacing)))
    if [ $yy -le 2 ]; then
        $yy=$yy+3
    fi
    for (( i=0; i<${#string_number}; i++ )); do

    digit="${string_number:$i:1}"
    case "$digit" in
        0) print_number $xx $(($yy+$i*4)) $color "${zero[@]}";;
        1) print_number $xx $(($yy+$i*4)) $color "${one[@]}" ;;
        2) print_number $xx $(($yy+$i*4)) $color "${two[@]}" ;;
        3) print_number $xx $(($yy+$i*4)) $color "${three[@]}" ;;
        4) print_number $xx $(($yy+$i*4)) $color "${four[@]}" ;;
        5) print_number $xx $(($yy+$i*4)) $color "${five[@]}" ;;
        6) print_number $xx $(($yy+$i*4)) $color "${six[@]}" ;;
        7) print_number $xx $(($yy+$i*4)) $color "${seven[@]}" ;;
        8) print_number $xx $(($yy+$i*4)) $color "${eight[@]}" ;;
        9) print_number $xx $(($yy+$i*4)) $color "${nine[@]}" ;;
    esac
done
tput cup $rows 1
echo -e "$(date +%d.%m.%Y)"
tput cup $(($rows-1)) $(($columns-10))
echo -e "$(date +%H:%M:%S)"

}

echo -e "Do we try to read  from file?    yes/anything else\t"
read answer
if [[ "$answer" == "yes" ]]; then
    test -f "time_left.txt" && read seconds < time_left.txt || read seconds
    test -f "time_left.txt" || touch time_left.txt
else
    echo -e "Input timer duration in seconds:\t"
    read seconds
fi

clear

#stty intr "^["

if [[ $seconds -gt 100 ]]; then
    seconds=10
elif [[ $seconds -lt 0 ]]; then
    second=10
fi

for ((counter=$seconds; counter>=1; counter--))
do
    echo "$counter" > time_left.txt
    color="white"
    if [ $counter -gt 9 ];
    then
        color="\e[32m"
    elif [ $counter -le 3 ];
    then
	color="\e[31m"
    else
	color="\e[33m"
    fi
    print_seconds_left $counter $color
    sleep 1
    clear
done

rm time_left.txt

x=$(($(tput lines) / 2))
y=$(($(tput cols) / 2 - 5))

tput cup $x $y

echo -e "\e[31mTIME OVER!\e[0m"
echo -e "\a"
sleep 0.2
echo -e "\a"
sleep 0.2
echo -e "\a"

sleep 2
clear
stty sane

