#!/bin/bash

sed -E '/[A-Za-z_][A-Za-z_0-9]*[ \t]+[1-9][0-9]*[ \t]+[1-9][0-9]*(\,[0-9]+)?/!d' data.txt > selected_data.txt
awk -F ' '  -v OFMT='%.2f' -v "LC_NUMERIC=C" 'BEGIN {
    total_amount=0
    total_price=0.0
    total_average=0.0
    printf "All data:\n\r"
    }
    {
    printf $0
    printf "\n"
    unique_goods[$1] += 1
    unique_sum[$1] += $2
    unique_price[$1] += $3*$2
    total_amount += $2
    total_price +=$3*$2
    }
    END {
    for (item in unique_goods)
    {
        if(unique_sum[item] != 0)
	{
	    unique_average[item] = unique_price[item]/unique_sum[item]
	}
    }
    # данные собраны в ассоциативные массивы => организуем вывод
    printf "\n\rResult:\n\r"

    printf "%-7s|%-15s|%-10s|%-10s|%-15s|%-15s", "№", "title", "number",
        "amount", "tot. price", "av. price"
    printf "\n\r"
    for (i = 1; i <= 75; i++) { printf "-"}
    printf "\n\r"
    nr=1
    for (item in unique_goods)
    {
        save_value = 0.0
        if (unique_sum[item] != 0)
	{
	    save_value = unique_price[item]/unique_sum[item]
	}
	printf "%-5d|%-15s|%-10d|%-10d|%-15s|%-15s", nr, item, unique_goods[item], 
            unique_sum[item], sprintf("%.4f", unique_price[item]), sprintf ("%.4f", save_value)
        nr+=1
        printf "\n\r"
    }
    for (i = 1; i <= 75; i++){ printf "-"}
    if (total_amount != 0)
    {
        total_average = total_price/total_amount
    }
    printf "\ntotal amount of goods: %-10d", total_amount
    printf "\ntotal price of goods: %-10s", sprintf("%.2f", total_price)
    printf "\naverage price of goods: %10s", sprintf("%.4f", total_average)
    printf "\n\r"
    }' selected_data.txt
