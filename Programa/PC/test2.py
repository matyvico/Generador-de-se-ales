#
# Example showing how QSignalMapper can be used to manage an arbitrary
# numbers of parameterless signals and re-emit them with an argument
# identifying the sender.
#
# Each signal is associated in the QSignalMapper with either an int,
# QString, QObject or a QWidget which is passed as argument to the slot
# connected to the QSignalMapper.
#
from PyQt4.QtCore import QSignalMapper, pyqtSignal
from PyQt4.QtGui import QApplication, QFrame, QGridLayout, QPushButton

class Grid(QFrame):
    """ Lay out widgets in a grid. """

    clicked =  pyqtSignal(int)
    """
    This signal will be emitted when on one of the grid items is clicked.
    The grid item's index will be passed as argument.
    """

    def __init__(self, items, colCount, parent=None):
        """
        Create Grid and setup QSignalMapper.

        Connect each item's 'clicked' signal and use the sequence index
        as map value which will be passed as argument when the Grid's
        clicked signal is emitted.

        items: sequence with widgets having a 'void clicked()' signal
        colCount: column count for each row
        parent: parent widget, default None
        """
        super(Grid, self).__init__(parent)

        # Create a grid layout.
        layout = QGridLayout()
        self.setLayout(layout)

        # Create the signal mapper.
        signalMapper = QSignalMapper(self)

        for cnt, item in enumerate(items):
            # Setup mapping for the item. In this case, the
            # mapping is the sequence index of the item.
            signalMapper.setMapping(item, cnt)

            # Connect the item's 'clicked' signal to the signal
            # mapper's 'map' slot.
            # The 'map' slot will emit the 'mapped' signal
            # when invoked and the mapping set previously will be
            # passed as an argument to the slot/signal that is
            # connected to the signal mapper's 'mapped' signal.
            item.clicked.connect(signalMapper.map)

            # Add the widget to the grid layout
            layout.addWidget(item, cnt / colCount, cnt % colCount)

        # Forward the signal mapper's 'mapped' signal via the Grid's
        # 'clicked' signal. This will handle all widgets' 'clicked'
        # ssignals.
        signalMapper.mapped.connect(self.clicked)

if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)

    # Create grid items
    items = (QPushButton('Open'), QPushButton('Close'),
             QPushButton('Read'), QPushButton('Write'),
             QPushButton('Delete'))

    win = Grid(items, 2)

    # Handle grid clicks here
    @win.clicked.connect
    def clicked(index):
        print ("Button at:", index, " - text:", items[index].text())

    win.show()
    sys.exit(app.exec_())
