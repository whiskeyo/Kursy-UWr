import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;

public class VehicleSwing extends JFrame
{
    JButton button1;
    JButton button2;

    JTextField textField1;
    JTextField textField2;
    JTextField textField3;

    JLabel label1;
    JLabel label2;
    JLabel label3;

    JTextArea textArea1;

    Vehicle vehicle;
    String objectName;

    public VehicleSwing(Vehicle vehicle, String objectName)
    {
        this.vehicle = vehicle;
        this.objectName = objectName;

        JFrame frame = new JFrame("Object of type Vehicle, name: " + objectName);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(500, 400);
    }
}