#!/bin/bash

random() {
    seed=$(($(date +%s) * 9301 + 49297))
    rand=$(($seed % 233280))
    echo $rand
}

print_seconds_left() {
    local secs=$1

    rows=$(tput lines)
    columns=$(tput cols)

    x=$(($(random) % ($rows - 2)))
    y=$(($(random) % ($columns - 8)))

    tput cup $x $y

    if [ $secs -gt 9 ]
    then
        echo -e "\e[32m$secs\e[0m"
    elif [ $secs -le 3 ]
    then
	echo -e "\e[31m$secs\e[0m"
    else
        echo -e "\e[33m$secs\e[0m"
    fi

    tput cup $rows 1
    echo -e "$(date +%d.%m.%Y)"
    tput cup $(($rows-1)) $(($columns-10))
    echo -e "$(date +%H:%M:%S)"

}

echo -e "Input timer duration in seconds:\t";

test -f "time_left.txt" && read seconds < time_left.txt || read seconds;
test -f "time_left.txt" || touch time_left.txt

clear

stty intr "^["

for ((counter=$seconds; counter>=1; counter--))
do
    echo "$counter" > time_left.txt
    print_seconds_left $counter
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

