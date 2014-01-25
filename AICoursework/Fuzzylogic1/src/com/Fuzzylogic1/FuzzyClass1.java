package com.Fuzzylogic1;

import net.sourceforge.jFuzzyLogic.FIS;
import net.sourceforge.jFuzzyLogic.FunctionBlock;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.text.*;

abstract class Shape {
 
	private double x, y; 
	private double dx, dy; 
	private Color color;
	private Rectangle box; 
 
	public Shape(double x, double y, Color color) {
		this.x = x; this.y = y;
		this.color = color;
	}
 
	public double getX() { return x; }
 
	public double getY() { return y; }
 
	public double getDX() { return dx; }
 
	public double getDY() { return dy; }
 
	public Color getColor() { return color; }
 
	public Rectangle getBoundingBox() { return box; }
 
	public void setBoundingBox(Rectangle box) {
		this.box = box;
	} 
  
	public void loopThrough(double dx, double dy) {
		this.dx = dx; this.dy = dy;
	}
 
	/** Move this shape (dx, dy)
	 */
	public void move() {
		x += dx; y += dy;
		constrain(); 
	}
 
	/** Keep this shape inside the bounding box.
	 *  Override this method in sub classes.
	 */
	protected void constrain() {
 
		double x0 = box.x, y0 = box.y;
		double x1 = x0 + box.width;
		double y1 = y0 + box.height;
 
		// If outside box, change direction
		if(x < x0) dx = Math.abs(dx);
		if(x > x1) dx = -Math.abs(dx);
		if(y < y0) dy = Math.abs(dy);
		if(y > y1) dy = -Math.abs(dy);
	} 
 
	/** Paint this shape (abstract method)
	 */
	abstract public void paint(Graphics g);
}

class Line extends Shape {
 
	private double x2;
	private double y2;
 
	public Line (double x, double y, Color color, double x2, double y2){
		super(x, y, color);
		this.x2 = x2;
		this.y2 = y2;
	}
 
	public double getX2(){
		return x2;
	}
 
	public double getY2(){
		return y2;
	}
 
	public void paint (Graphics g){
		g.setColor(Color.red);
		g.drawLine((int) getX(), (int) getY(), (int) getX(), (int) y2);
	}
 
}

class Circle extends FillableShape {
	 
	private double diameter;
 
	public Circle (double x, double y, Color color, double diameter){
		super(x, y, color);
		this.diameter = diameter;
	}
 
	public double getDiameter(){
		return diameter;
	}
 
	public void paint(Graphics g){
		g.setColor(Color.orange);
		g.fillOval((int)getX(), (int)getY(), (int)diameter, (int)diameter);
		g.setColor(Color.black);
		g.drawOval((int)getX(), (int)getY(), (int)diameter, (int)diameter);
	}
	
}

abstract class FillableShape extends Shape
{
	private boolean filled;
	
	public FillableShape(double x, double y, Color color)
	{
		super(x, y, color);
	}
	
	public void setFilled(boolean v)
	{
		filled = v;
	}
	
	public boolean getFilled()
	{
		return filled;
	}
}


/** 
 * FuzzyControllerPanel represents a canvas (drawing area) for objects of
 * type Line and Circle (Road and Car).
 * A thread-object periodically updates the canvas (in the run method), 
 * by moving and repainting the objects (the animation).  
 */
 
class FuzzyControllerPanel extends JPanel implements Runnable, ActionListener {
 
	private int width = 600, height = 480;
	private Thread thread;
	private Line line;
	private Line hLine1, hLine2;
	private Circle circle;
	private double maxVel = 4.90f, minVel = 0.0f; 
//	FIS fis;
	long startTime, endTime;	
	private volatile boolean running = true;
	
	public void actionPerformed(ActionEvent e) {}
	
	public FuzzyControllerPanel() {
		
		startTime = System.nanoTime();
		
		int x1 = 0, x2 = width/2, x3 = height/2, dist = 40, radius = 40;
		Rectangle box = new Rectangle(width, height);
 
		Color color = new Color(255, 0, 0);
 
		hLine1 = new Line(0, 240, color, 500, 240);
		hLine2 = new Line(x1, x3 + dist, color, width, x3 + dist);
		hLine1.setBoundingBox(box);
		hLine2.setBoundingBox(box);
 
		hLine1.loopThrough(0.0, 0.0);
		hLine2.loopThrough(0.0, 0.0);
		
		line = new Line(x2, x3 - dist, color, x2, x3 + dist);
	//	line = new Line(0, x3 - dist, color, 0, x3 + dist);
		circle = new Circle(x2, x3 - dist + 20, color, radius);
	
		line.setBoundingBox(box);
		circle.setBoundingBox(box);
 
		line.loopThrough(VelocityValue.MAXVEL.getVelValue(), 0.0);
		circle.loopThrough(1.0, 0.0);
 
		// Set the the dimensions of the drawing area 
		setPreferredSize(new Dimension(width, height));
		
	} 
	
	public void startThread(){
		// Create the thread-object, responsible for the animation
		running = true;
		thread = new Thread(this);
		thread.start();
	}
 
	/** Define what to draw. Called by repaint(). 
	 */
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		hLine1.paint(g);
		hLine2.paint(g);
		line.paint(g);
		circle.paint(g);
	}
 
	/** Update the position of the ball and repaint.
	 *  This method is executed by the thread-object.
	 */
	public void run() {
		// high vel = 1.0f, low = 0.5f
		double vel = 0.0f;
		double relDist = 0.0f;
		double circleSteer = 0.0f;
		DecimalFormat df = new DecimalFormat("#.##"); 
		
		while(running) {
 
			if (width != getWidth() || height != getHeight()){
				width = getWidth(); height = getHeight();}
 
			Rectangle box = new Rectangle(width, height);
 
			hLine1.setBoundingBox(box);
			hLine2.setBoundingBox(box);
			
			line.setBoundingBox(box);
			circle.setBoundingBox(box);		
			
			line.move();
			circle.move();
	
			repaint(); // Calls paintComponent(g)
 
			// Sleep for 20 ms before next update
			try {
				Thread.sleep(20);
				endTime = System.nanoTime();
				long currTime = endTime - startTime;
				
				setVelocity();
				relDist = getRelDistance();
				circleSteer = setSteer(relDist);
				circle.loopThrough(circleSteer, 0.0f);
				
			//	System.out.println("circlesteer:" + circleSteer + "  dx:" + line.getDX() + "  x:" + line.getX() + " c:" + circle.getX() + " rel:" + relDist + " time:" + currTime);
				System.out.println("circle :" + df.format(circle.getX()) + " line : " + df.format(line.getX()));	
			}
			catch(InterruptedException e) {}
		}
	}
	
	public void terminate() {
        running = false;
    }
	
	static enum VelocityValue{
		MAXVEL(5.0),
		HIGHVEL(4.0),
		MIDVEL(1.67),
		LOWVEL(0.83),
		MINVEL(0.42);
		
		private final double velocity;
		VelocityValue(double value){
			this.velocity = value;
		}
		
		public double getVelValue(){
			return this.velocity;
		}

	};
	
	static enum DistanceValue{
		DIST1(5.0),
		DIST2(25.0),
		DIST3(50.0),
		DIST4(100.0),
		DIST5(500.0),
		DIST6(550.0),
		DIST7(575.0),
		DIST8(595.0);
		
		private final double dist;
		DistanceValue(double dist){
			this.dist = dist;
		}
		
		public double getDistance(){
			return this.dist;
		}
	}
	
	private static boolean IsPositive(double value){
		return (value>0)?true:false;
	}
	
	private static boolean IsDistLess(Double val, Double dist){
	    return (dist <= val)?true:false;
	}
	
	private static boolean IsDistMore(Double val, Double dist){
	    return (dist >= val)?true:false;
	}
	
	private static void centerToLeft(double dist, double vel, Line line){
		if(IsDistLess(DistanceValue.DIST4.getDistance(), dist) && !IsPositive(vel))
			line.loopThrough(-VelocityValue.MIDVEL.getVelValue(), 0.0);
		if(IsDistLess(DistanceValue.DIST3.getDistance(), dist) && !IsPositive(vel))
			line.loopThrough(-VelocityValue.LOWVEL.getVelValue(), 0.0);
		if(IsDistLess(DistanceValue.DIST2.getDistance(), dist) && !IsPositive(vel))
			line.loopThrough(-VelocityValue.MINVEL.getVelValue(), 0.0);
	}
	
	private static void leftToRight(double dist, double vel, Line line){
		if(IsDistMore(DistanceValue.DIST1.getDistance(), dist) && IsPositive(vel))
			line.loopThrough(VelocityValue.MAXVEL.getVelValue(), 0.0);
		if(IsDistMore(DistanceValue.DIST5.getDistance(), dist) && IsPositive(vel))
			line.loopThrough(VelocityValue.MIDVEL.getVelValue(), 0.0);
		if(IsDistMore(DistanceValue.DIST6.getDistance(), dist) && IsPositive(vel))
			line.loopThrough(VelocityValue.LOWVEL.getVelValue(), 0.0);
		if(IsDistMore(DistanceValue.DIST7.getDistance(), dist) && IsPositive(vel))
			line.loopThrough(VelocityValue.MINVEL.getVelValue(), 0.0);
	}
	
	private static void rightToLeft(double dist, double vel, Line line){
		if(IsDistLess(DistanceValue.DIST8.getDistance(), dist) && !IsPositive(vel))
			line.loopThrough(-VelocityValue.MAXVEL.getVelValue(), 0.0);
		centerToLeft(dist, vel, line);
	}
	
	private void setVelocity(){
		
		// based on distance from the center, velocity is set, if near boundaries
		// velocity is less, else in middle it is high
		double dist = line.getX();
		double vel = line.getDX();
		
	//	centerToLeft(dist, vel, line);
		leftToRight(dist, vel, line);
		rightToLeft(dist, vel, line);
	}
 
	private double getRelDistance(){
		double lineX = line.getX();
		double circleX = circle.getX();
		
		return (lineX - circleX);
		//double dist = 
	}
	
	private double setSteer(double dist){
		// velocity as of now is -100 to 100, so to map
		// 5 -> 90 (which means 18 times, so) 1 -> 18
		double velMap = 20.0f;//20.36f; // as max vel is 4.91 in range of -100 to 100, so 100/20.36 = 4.91
		double vel = line.getDX() * velMap;
		
		// distance is -300 to 300, in fcl it's -100 to 100
		// so divide by 3 (?)
		double distMap = 3.0f;
		dist = (dist/distMap);
		
		
		return getSteerValue(vel, dist);
		
	}
	
	private static FIS loadFuzzyFile(){
		String filename = "aiassess.fcl";
		FIS fis = FIS.load(filename, true);

		if (fis == null) {
			System.err.println("Can't load file: '" + filename + "'");
			System.exit(1);
		}
		
		return fis;
	}
	
	public static double getSteerValue(double vel, double dist){
		String filename = "aiassess.fcl";
		FIS fis = FIS.load(filename, true);
		double mapVal =  10.0f; // too slow 20.36f;
		
		
		if (fis == null) {
			System.err.println("Can't load file: '" + filename + "'");
			System.exit(1);
		}
		// Get default function block
		FunctionBlock fb = fis.getFunctionBlock(null);
		fb.setVariable("distance", vel);
		fb.setVariable("velocity", dist);
		fb.evaluate();
		fb.getVariable("steer").defuzzify();
		double steer = fb.getVariable("steer").getValue();
//		System.out.println("Steer is:	vel: " + vel + " dist: " + dist + " Steer: " + steer );
		return steer/mapVal;
	}
	
	private static final long serialVersionUID = 1L;
}



public class FuzzyClass1 implements ActionListener {

	public void actionPerformed(ActionEvent e) {
	   
	} 
	
	public static void main(String[] args) throws Exception {
		
		
		JButton startAnim, stopAnim, calcSteer;
	//	long startTime = System.currentTimeMillis();
		
		// Create a window (frame)
		final JFrame frame = new JFrame("Fuzzy Car Controller");
	//	frame.setLayout(new GridBagLayout());
		
		JPanel south = new JPanel(new GridLayout(0,1,2,2));
		JPanel north = new JPanel(new GridLayout(4, 2));
		
		startAnim = new JButton("Start");
        startAnim.setBounds(300, 20, 100, 50);
        
        stopAnim = new JButton("Stop");
        stopAnim.setBounds(100, 400, 100, 50);
        
        south.add(startAnim, BorderLayout.PAGE_START);
        south.add(stopAnim, BorderLayout.PAGE_END);
        
        JLabel velLabel = new JLabel("Velocity");
        JLabel distLabel = new JLabel("Distance");
        final JLabel steerLabel = new JLabel("Steer");
        JLabel none = new JLabel();
        final JTextField inputVel = new JTextField();
        final JTextField inputDist = new JTextField();
        
        calcSteer = new JButton("Calculate");
        calcSteer.setBounds(100, 400, 100, 50);
    //   calcSteer.setPreferredSize(new Dimension(100, 50));
       
        north.add(velLabel);
        north.add(inputVel);
        north.add(distLabel);
        north.add(inputDist);
        
        north.add(calcSteer);
        north.add(none);
        north.add(steerLabel);
        
        final FuzzyControllerPanel panel = new FuzzyControllerPanel();
		frame.getContentPane().add(panel);
		
		frame.add(south, BorderLayout.PAGE_END);
		frame.add(north, BorderLayout.PAGE_START);
		
        startAnim.addActionListener(new ActionListener(){ 
            public void actionPerformed(ActionEvent e)
            {
            	// Create a canvas (BouncePanel) and add it to the window
            	panel.startThread();
            }
        });
        
        stopAnim.addActionListener(new ActionListener(){
        	public void actionPerformed(ActionEvent e){
        		panel.terminate();
        	}
        });
        
        calcSteer.addActionListener(new ActionListener(){
        	public void actionPerformed(ActionEvent e){
        		double vel = 0.0f, dist = 0.0f, steer = 0.0f;
        		String sVel, sDist;
        		
        		sVel = inputVel.getText();
        		sDist = inputDist.getText();
        		
        		// convert the string to double
        		vel = Double.parseDouble(sVel);
        		dist = Double.parseDouble(sDist);
        		// call fn, *10.0. getSteerValue(vel, dist), get steer * 10;
        		steer = FuzzyControllerPanel.getSteerValue(vel, dist);
        		steer *= 10.0f;
        		// set the steer value in the label
        		steerLabel.setText("Steer is: " + steer);
        	}
        });
        
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.pack();
		frame.setVisible(true);	

	//	long endTime = System.currentTimeMillis();

	//	System.out.println("That took " + (startTime) + " milliseconds");
			

	}
	
	
}
