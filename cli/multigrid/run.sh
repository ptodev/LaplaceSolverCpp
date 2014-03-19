echo
echo "########### INPUT FILE ############"
cat $1
echo "###################################"
echo

python check.py $1
if [ $? -eq 1 ]
	then
		exit
fi

./solver $1 $2
python plot.py $1 $2 &
python query.py $1 $2
