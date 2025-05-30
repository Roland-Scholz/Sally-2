

$fn = 20;

thick = 2;
xmax = 250;
ymax = 180+thick;
zmax = 39;

edge=5;

//xmt = xmax + 2*thick;
//ymt = ymax + 2*thick;    

    
difference() {
    shell_top();

    //holes for screws pcb
    /*
    translate([20, 35,0])
        cylinder(4, 3.5, 3.5);
    translate([xmax-20,35,0])
        cylinder(4, 3.5, 3.5);
    translate([21.5,35+118,0])
        cylinder(4, 3.5, 3.5);
    translate([xmax-21.5,35+118,0])
        cylinder(4, 3.5, 3.5);
    */
    
    
    //bumper
    /*
    translate([25, 20, 0])
        cylinder(1, 1, 1);
    translate([xmax-25, 20,0])
        cylinder(1, 1, 1);
    translate([25, ymax-15, 0])
        cylinder(1, 1, 1);
    translate([xmax-25, ymax-15, 0])
        cylinder(1, 1, 1);
    */    
     //text("Sally-2");
    
}

module atarilogo() {
    translate([0,35,0])
        base_shape_a();
}

module ataritext() {
    linear_extrude(0.4)
        mirror([1,0,0])
        text("ATARI ", 16, "SF Atarian              System:style=Regular");

    translate([-45, 0, 0])
    linear_extrude(0.4)
        mirror([1,0,0])
    text("SALLY-2 ", 12, "SF Atarian System:style=Regular");
}

module base_shape_a() {
        rotate([90,0,0]) 
            linear_extrude(20)
                base_shape_2d_a(1.2);
}

module base_shape_2d_a(th) {
    polygon([/* [0, zmax], */ [0, edge], [edge,0], [xmax-edge, 0], [xmax, edge], /*[xmax, zmax],[xmax-th,zmax],*/ [xmax-th, edge+th], [xmax-edge-th, th],[edge+th, th], [th, edge+th]/*,[th, zmax]*/]);
}




module shell_top() {
   
//    atarilogo();
    
    difference(){
        base_shape();
        cubes_left();    
        cubes_right();
        atarilogo();
        
        //holes for screws case
        /*
        translate([edge+5,28,0])
            cylinder(4, 3.5, 3.5);
        translate([xmax-edge-5,28,0])
            cylinder(4, 3.5, 3.5);
        translate([edge+5,ymax-26,0])
            cylinder(4, 3.5, 3.5);
        translate([xmax-edge-5,ymax-26,0])
            cylinder(4, 3.5, 3.5);
        */
        


    }

    //clips left right
    translate([thick/2,3*thick,zmax-1])
        cube([thick/2,ymax-6*thick, 3]);
    translate([xmax-thick,3*thick,zmax-1])
        cube([thick/2,ymax-6*thick, 3]);


    translate([0, ymax+thick])
        base_shape_holder();
    translate([0, ymax-thick])
        base_shape_holder();
    translate([0, 0])
        base_shape_holder();
    translate([0, 0+2*thick])
        base_shape_holder();    


    
    translate([0, 26, 10])
        cube([8,4,25]);
    translate([0, ymax-28, 10])
        cube([8,4,25]);
    translate([xmax-edge-2*thick+1, 26, 10])
        cube([8,4,25]);
    translate([xmax-edge-2*thick+1, ymax-28, 10])
        cube([8,4,25]);
    
    translate([3.5*thick, 17, 1.2])
        cube([xmax-7*thick, 20, 3]);
    //feet, set PCB on top
    /*
    translate([20,35,0])
        foot(3, 22);
    translate([xmax-20,35,0])
        foot(3, 22);
        
    translate([21.5,35+118,0])
        foot(3, 22);
    translate([xmax-21.5,35+118,0])
        foot(3, 22);
    */
}




module base_shape() {
    translate([0, ymax+thick, 0])
        rotate([90,0,0]) 
            linear_extrude(ymax+thick)
                base_shape_2d(thick);

    //screws holding case together
    translate([edge+5,28,0])
        screw(2.5);
    translate([xmax-edge-5,28,0])
        screw(2.5);
    translate([edge+5,ymax-26,0])
        screw(2.5);
    translate([xmax-edge-5,ymax-26,0])
        screw(2.5);
    
    //reinforcement front
    translate([0, 17 ,0])
        base_shape_holder();    
    
    //reinforcement in the middle
    translate([0, ymax/2 - 10])
        base_shape_holder();    
    translate([0, ymax/2 + 14])
        base_shape_holder();    

    //reinforcement back
    translate([0, ymax-14 ,0])
        base_shape_holder();    

}

module base_shape_holder() {
    
    rotate([90,0,0])
    linear_extrude(thick)
        base_shape_2d(3*thick);

}

module base_shape_2d(th) {
    polygon([[0, zmax], [0, edge], [edge,0], [xmax-edge, 0], [xmax, edge], [xmax, zmax],[xmax-th,zmax], [xmax-th, edge+th], [xmax-edge-th, th],[edge+th, th], [th, edge+th],[th, zmax]]);
}


module cubes_left() {
    for(i = [40:8:ymax-14])
      translate([0, i, 0])
      cube([edge+thick, 4, edge+thick]);
}

module cubes_right() {
    for(i = [40:8:ymax-14])
      translate([xmax-edge-thick, i, 0])
      cube([edge+thick, 4, edge+thick]);
}
    
module screw(r) {    
    difference() {
        cylinder(zmax,r*2,r*2);
        translate([0,0,-1]);
            cylinder(zmax+2, 2.0, 2.0);
//        cylinder(5,6,6);
    }
}

module foot(r, h) {    
    difference() {
        cylinder(h,r*2,r*1.3);
            cylinder(h,r/2+0.1,r/2+0.1);
//        cylinder(5,6,6);
    }
}