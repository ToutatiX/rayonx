echo "Please type one of the following commands:"
echo "  'fp' to preview the frontend in localhost"
echo "  'up' to upload on the board"
echo "  'bu' to compile the programm"
echo "  'mt' to monitor"
echo

while :
do
  read INPUT_STRING
  case $INPUT_STRING in
	fp)
        cd ./frontend/
        npm start
		break
		;;
	up)
        cd ./backend/
        pio run -t upload -e esp32Front
		break
		;;
  bu)
      cd ./backend/
      pio run
  break
  ;;
  mt)
      cd ./backend/
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