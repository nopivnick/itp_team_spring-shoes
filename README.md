# itp_team_spring-shoes

## Instructions to run the p5 sketch on a localhost

- If running Arduino IDE, make sure serial monitor is closed
- Run P5.serialcontrol.app
- Select serial port w/ Arduino from dropdown menu
- Open serial port
- Enable serial port
- Check P5.serialcontrol.app for the computer's IP address
- Confirm `serial = new p5.SerialPort("<ip-address>")` in `function setup()` is set correctly
- Open Terminal
- `$ cd ./<project-directory>`
- `$ python -m SimpleHTTPServer`
- Open Chrome and enter `http://<ip-address>:8000`
