echo "Please type one of the following commands:"
echo "  'fp' to preview the frontend in localhost"
echo "  'up' to upload on the board"
echo "  'er' erase flash"
echo "  'bu' to compile the programm"
echo "  'mt' to monitor"
echo

while :
do
  read INPUT_STRING
  case $INPUT_STRING in
	fp)
      cd ./interface/
      npm start
		  break
		;;
	up)
      pio run -t upload
		  break
		;;
  er)
      pio run -t erase
		  break
		;;
  bu)
    pio run -e node32s_back
    break
  ;;
  mt)
    pio device monitor
    break
  ;;
	*)
		echo "Command unknown -> exiting script"
    break
	;;
  esac
done
echo 