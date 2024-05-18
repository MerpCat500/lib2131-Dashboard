import sys, random  
import numpy as np
import pyqtgraph as pg

from PIL import Image

# Graphics Stuffs (PYQT6)
from PyQt6.QtCore import Qt, QSize, QTimer, QPoint
from PyQt6.QtWidgets import (
    QMainWindow,
    QWidget,
    QApplication,
    QPushButton,
    QVBoxLayout,
    QLabel,
)

# A Field that can be embeded into the main window
class Field(QWidget):
    def __init__(self) -> None:
        super().__init__()

        self.pos = None
        self.speed = None
        self.cursor_pos = QPoint(0, 0)

        # Make Graph, Set size, Name, No user interaction
        self.Graph = pg.PlotWidget()
        self.Graph.plotItem.vb.setRange(xRange=[0, 144], yRange=[0, 144], padding=0)

        self.Graph.plotItem.vb.disableAutoRange()
        self.Graph.plotItem.vb.setMouseEnabled(False, False)
        self.Graph.plotItem.hideButtons() 
        self.Graph.plotItem.setTitle("Robot Position")
        
        # Setup Callback/Signal for mouse position
        self.Proxy = pg.SignalProxy(
            self.Graph.scene().sigMouseMoved, rateLimit=60, slot=self.mouseMoved
        )

        # Background image
        img = Image.open("Lib213-Dashboard/high stakes.png")
        img = img.resize((144, 144), Image.BILINEAR)
        np_img = np.array(img)
        self.img = pg.ImageItem(np_img)
        self.Graph.plotItem.vb.addItem(self.img)

        # Adjust Axis, Show Graph
        self.Graph.plotItem.showGrid(True, True, 1)
        axis_bottom: pg.AxisItem = self.Graph.plotItem.getAxis("bottom")
        axis_bottom.setTickSpacing(24, 12)
        axis_left: pg.AxisItem = self.Graph.plotItem.getAxis("left")
        axis_left.setTickSpacing(24, 12)

        # Timer
        self.Graph.update_timer = QTimer()
        self.Graph.update_timer.setInterval(50)
        self.Graph.update_timer.timeout.connect(self.refresh)
        self.Graph.update_timer.start()

        # Labels
        self.Label = QLabel("Robot Position: N/A, Robot Speed: N/A")

        # Layout
        self.layout: QVBoxLayout = QVBoxLayout()
        self.layout.addWidget(self.Graph)
        self.layout.addWidget(self.Label)
        self.layout.addStretch()
        self.setLayout(self.layout)

        # Force Size
        self.Graph.setFixedSize(QSize(600, 600))

    # When mouse is moved while above the Graph Widget
    def mouseMoved(self, event):
        pos = event[0] # Current mouse Pos
        if self.Graph.sceneBoundingRect().contains(pos): # If within Field
            self.cursor_pos = self.Graph.getPlotItem().vb.mapSceneToView(pos).toPoint() # Convert to a point (int)

    # Planning to call this every time the Client gets new info, That way it only runs as fast as the robot is.
    def refresh(self) -> None:

        # Robot pos/speed (Random For testing)
        self.pos = (self.cursor_pos.x(), self.cursor_pos.y())    
        self.speed = (random.randint(0, 144), random.randint(0, 144))

        # Clear Graph
        self.Graph.plotItem.clearPlots()
        
        # If a Pos exists (ie connected to robot)
        if self.pos is not None:
            # Graph robot info
            self.Graph.plot(
                [self.pos[0]],
                [self.pos[1]],
                pen=None,
                symbol="+",
                symbolSize=22,
                symbolBrush=pg.mkBrush("w"),
            )
            # Display info as text
            self.Label.setText(
                f"Current Robot Position: ({self.pos[0]}, {self.pos[1]})\n"
                f"Current Robot Speed: ({self.speed[0]}, {self.speed[1]})\n"
                f"Mouse Position: ({self.cursor_pos.x()}, {self.cursor_pos.y()})"
            )

# Starting on QUI for other devices
class vexDevice(QWidget):
    def __init__(self) -> None:
        super().__init__()

# Main Window for user Applications
class MainWindow(QMainWindow):
    def __init__(self) -> None:
        super().__init__()
        self.Field = Field()

    def show_new_window(self, checked):
        self.w = Field()
        self.w.show()

    def show(self):
        self.Field.show()

# App Instance (Args allow for commands)
app = QApplication(sys.argv)

# Main Window
win = MainWindow()
win.show()

# Main Loop
app.exec()
