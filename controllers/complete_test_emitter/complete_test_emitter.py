from controller import Robot


robot = Robot()
timestep = int(robot.getBasicTimeStep())
emitter = robot.getDevice('emitter')
while robot.step(timestep) != -1:
    emitter.send(b'test')
