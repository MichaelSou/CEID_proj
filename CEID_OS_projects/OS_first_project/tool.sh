#!/bin/bash


args=("$@") #array pou periexei ola ta 
num_args=("$#") #ari8mos ton 

file_exist() #h sunarthsh elegxei an yparxei to arxeio pou dinoume sto script
{
	local name = $1
	[[ -f $name ]] && return 0 || return 1
}


read_file() #h sunarthsh emfanizei olo to arxeio ektos apo ta sxolia 
{
	while read line
	do
		if [[ ${line:0:1} != "#" ]] ; then echo $line ; fi
	done < $1
}

search_id()
{
	local file=$1
	local arg=$2
	while read line #pairnoume mia mia tis grammes tou arxeiou
        do
                if [[ ${line:0:1} != "#" ]] #elegxei an h grammh den einai sxolio
                then 
                      cmp="$(cut -d'|' -f1 <<<"$line")" #bazoume sthn metavlith cmp to id ths grammhs
			if [ "$cmp" == "$arg" ] #elegxoume an to id pou parexoume einai idio me auto ths grammhs
			then
				#emfanizoume to eponimo, to onoma kai thn hmer gennhshs tou atomou me to id pou psaxnoume
				echo -ne  "$(cut -d'|' -f2 <<<"$line") "
				echo -ne "$(cut -d'|' -f3 <<<"$line") "
				echo "$(cut -d'|' -f5 <<<"$line")"
			fi  
                fi
        done < $file
}

findNcount()
 {
         local file=$1
         local arg=$2
	 local countt=('')
	 Array=(' ')
	 local exist=0
	 element=""
         while read line
         do
		 exist=0
                 if [[ ${line:0:1} != "#" ]] #elegxoume an h grammh einai sxolio
                 then
			#elegxoume to orisma gia na doume poia entolh 8a ektelesoume
			if [ "$arg" == "--firstnames" ]
			then
				element="$(cut -d'|' -f3 <<<"$line")"
			elif [ "$arg" == "--lastnames" ]
			then
				element="$(cut -d'|' -f2 <<<"$line")"
			elif [ "$arg" == "--socialmedia" ]
			then
				element="$(cut -d'|' -f9 <<<"$line")"	
			fi
			for i in ${!Array[@]} #me auth thn for elegxoume an h egrafh uparxei sto array kai auksanoume to countt an den uparxei thn bazoume sto array
                        do
				if [ "$element" == "${Array[i]}" ]
                                then
					((countt[$i]++))
					exist=1
                                fi
                        done
                        if [ "$exist" -eq  0 ] 
                        then
				 Array+=("$element")    
                        fi
                 fi
        done < $file
	
	for a in ${!Array[@]}#me auth th for emfanizoume to array
	do
		echo "${Array[$a]}"
	if [ "$arg" == "--socialmedia" ] #an to orisma tou script einai --socialmedia emfanizoume poses idies egrafes uparxoun
	then
		echo " ${countt[$a]}"
	fi
	done
 }


search_date()
{
	
	
        	local file=$1
        	local A=$2
		local B=$3
		if [ "$A" != 0 ] #an exoume dosei to orisma --born-since meta trepoume thn hmeromhnia se deuterolepta
		then
			A=$(date -d $A +%s)
		fi
		if [ "$B" != 0 ] #an exoume dosei to orisma --born-until meta trepoume thn hmeromhnia se deuterolepta
		then
			B=$(date -d $B +%s)
		fi
        	while read line
        	do
                	if [[ ${line:0:1} != "#" ]] #elegxoume an h grammh einai sxolio
                	then
                      		dt="$(cut -d'|' -f5 <<<"$line")" #bazoume sthn metavlhth dt thn hmeromhnia gennhshs ths egrafhs
				cmp=$(date -d $dt +%s) #metatrepoume thn hmeromhnia se deuterolepta
				#me ton parakato kodika elegxoume ta orismata kai emfanizoume tis hmeromhnies pou entasonte sta kritiria
				if [ "$cmp" -ge  "$A" ] && [ "$B" -eq 0 ]
				then
					echo "$line"
				elif [ "$cmp" -lt  "$B" ] && [ "$A" -eq 0 ]
				then
					echo "$line"
				elif [ "$cmp" -ge  "$A" ] && [ "$cmp" -le "$B" ]
                        	then
                                	echo "$line"
                        	fi
                	fi
        	done < $file
}

edit()
{
	local file=$1
	local id=$2
	local column=$3
	local value=$4

        while read line
        do
                if [[ ${line:0:1} != "#" ]] #elegxoume an h grammh einai sxolio h oxi
                then 
                      cmp="$(cut -d'|' -f1 <<<"$line")" #pairnoume to id ths egrafhs
                        if [ "$cmp" == "$id" ]
                        then
                                case $column in #me auth thn case elegxoume poio column 8a alaksoume
					1 )
						echo "can't change id" ;; #den mporoume na alaksoume to id!
					2 )
						old_value="$(cut -d'|' -f2 <<<"$line")"
						new_line=$(sed -i 's/"$old_value"/"$value"/g' "$line")
						sed -i '/"$id"/s/"$line"/"$new_line"/' "$file"
						;;
					3 )
						old_value="$(cut -d'|' -f3 <<<"$line")"
                                                new_line=sed -i 's/$old_value/$value/g' $line
                                                sed -i '/$id/s/$line/$new_line/' $file
						;;
					4 )
						old_value="$(cut -d'|' -f4 <<<"$line")"
                                                new_line=sed -i 's/$old_value/$value/g' $line
                                                sed -i '/$id/s/$line/$new_line/' $file
						;;
					5 )
						old_value="$(cut -d'|' -f5 <<<"$line")"
                                                new_line=sed -i 's/$old_value/$value/g' $line
                                                sed -i '/$id/s/$line/$new_line/' $file
						;;
					6 )
						old_value="$(cut -d'|' -f6 <<<"$line")"
                                                new_line=sed -i 's/$old_value/$value/g' $line
                                                sed -i '/$id/s/$line/$new_line/' $file
						;;
					7 )
						old_value="$(cut -d'|' -f7 <<<"$line")"
                                                new_line=sed -i 's/$old_value/$value/g' $line
                                                sed -i '/$id/s/$line/$new_line/' $file
						;;
					8 )
						old_value="$(cut -d'|' -f8 <<<"$line")"
                                                new_line=sed -i 's/$old_value/$value/g' $line
                                                sed -i '/$id/s/$line/$new_line/' $file
						;;
					9 )
						old_value="$(cut -d'|' -f9 <<<"$line")"
                                                new_line=sed -i 's/$old_value/$value/g' $line
                                                sed -i '/$id/s/$line/$new_line/' $file
						;;
					esac
                        fi
                fi
        done < $file

	
}




case $num_args in #me authn thn case elegxoume ton ari8mo ton orismaton
	0 )
		echo "1054434-1054420" ;; #an den exoume orismata emfanizontai ta AM
	2 )
		if [ "file_exist $2" ] && [ "${args[0]}" == "-f"  ] #an ta orismata einai 2 kai to proto einai -f pairnoume to epomeno to opoio einai to arxeio
	        then
			 read_file $2  #kaloume thn read_file me orisma to arxeio
		else
			 echo "File not found or wrong command" 
		fi ;;
	3 )
		for i in "${!args[@]}" #auth h for elegxei ta orismata gia na mporoume na ta eisagoume me opoio seira 8eloume
                do
                        if [ "${args[i]}" == "-f" ]
                        then
                                file=${args[i+1]}
                        elif [ "${args[i]}" == "--firstnames" ]
                        then
                                mode=${args[i]}
			elif [ "${args[i]}" == "--lastnames" ]
                        then
                                mode=${args[i]}
			elif [ "${args[i]}" == "--socialmedia" ]
                        then
                                mode=${args[i]}
                        fi
                done
                if [ "file_exist $file" ] #an to arxeio uparxei ekteloume thn findNcount
                then
                        findNcount "$file" "$mode"
                else
                         echo "File not found" 
                fi ;;
	4 )
		id=0
		dateA=0
		dateB=0
		for i in "${!args[@]}" # h for elegxei ta orismata gia na mporoume na ta eisagoume me opoia seira 8eloume
		do
			if [ "${args[i]}" == "-f" ]
			then
				file=${args[i+1]}	
			elif [ "${args[i]}" == "-id" ]
			then
				id=${args[i+1]}
			elif [ "${args[i]}" == "--born-since" ]
                        then
                                 dateA=${args[i+1]}
                        elif [ "${args[i]}" == "--born-until" ]
                        then
				dateB=${args[i+1]}
			fi 
		done
		if [ "file_exist $file" ] #elegxoume an to arxeio uparxei
                then
                	if [ "$id" -eq 0 ] #an to id einai 0 shmainei oti exoume dosei orismata gia na treksei h search_date allios trexei h search_id
			then
				search_date "$file" "$dateA" "$dateB"
			else
				search_id "$file" "$id"
			fi
                else
                         echo "File not found" 
                fi ;;
	6 )
		id=0
		dateA=0
		dateB=0
		for i in "${!args[@]}" #h for elegxei ta orismata gia na mporoume na ta eisagoume me opoia seira 8eloume
                do
                        if [ "${args[i]}" == "-f" ]
                        then
                                file=${args[i+1]}
                        elif [ "${args[i]}" == "--born-since" ]
                        then
                                dateA=${args[i+1]}
                        elif [ "${args[i]}" == "--born-until" ]
                        then
                                dateB=${args[i+1]}
			elif [ "${args[i]}" == "--edit" ]
                        then
                                id=${args[i+1]}
				column=${args[i+2]}
				value=${args[i+3]}
                        fi
                done
                if [ "file_exist $file" ]#elegxoume an to arxeio uparxei
                then
                     if [ "$id" -gt 0 ] && [ "$dateA" == 0 ] && [ "$dateA" == 0  ] #elegxoume ta orismata gia na doume poia sunarthsh 8a treksei
		     then
				edit "$file" "$id" "$column" "$value"
		     elif [ "$id" -eq 0 ]
		     then
				search_date "$file" "$dateA" "$dateB"
		     fi
                else
                         echo "File not found" 
                fi ;;
	* )
		echo "unknown command" ;; #an baloume la8os ari8mo orismaton den 8a treksei to programa
esac


