import RobotWindow from 'https://cyberbotics.com/wwi/R2023b/RobotWindow.js';
window.robotWindow = new RobotWindow();
window.onload = () => {
  window.robotWindow.receive = function (message, robot) {
    if (message.startsWith('test')) {
      document.querySelector("#text-display").innerHTML = message;
      this.send("Answer: " + message, robot);
    } else
      console.log("Received unknown message for robot '" + robot + "': '" + message + "'");
  }
}
