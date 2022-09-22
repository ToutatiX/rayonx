echo "Please type one of the following commands:"
echo "  'fp' to preview the frontend in localhost"
echo "  'up' to upload on the board"
echo "  'bu' to compile the programm"
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
        pio run --target upload
		break
		;;
    bu)
        cd ./backend/
        pio run
		break
		;;
	*)
		echo "Command unknown -> exiting script"
        break
		;;
  esac
done
echo 