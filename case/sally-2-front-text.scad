$fn = 20;

thick = 2;
tf = 1.2;

xmax = 250;
ymax = 180+thick;
zmax = 39*2;

ledx = 142;
ledy = 12;

edge=5;

//rotate([180,0,0]) {
//    shape();
//}
beschriftung();

module shape() {
    
    echo("Floppy start:", 14);
    echo("Switch start:", 32);
    echo("Switch start:", 75);
    echo("Switch start:", 94);
    
    difference(){
        base_shape();
        
        translate([32, 10, 0])
            switch();
        translate([76, 10, 0])
            switch();
        translate([93.5, 10, 0])
            switch();
        
        translate([216, 26.5, 0])
            reset();
        
        translate([12.5, 35, 0])
            floppy();
        translate([128, 35, 0])
            floppy();

        translate([ledx, ledy, 0])
            led3();
        translate([ledx+15, ledy, 0])
            led3();
        translate([ledx+2*15, ledy, 0])
            led3();
            
        translate([ledx, ledy+15, 0])
            led3();
        translate([ledx+15, ledy+15, 0])
            led3();
        translate([ledx+2*15, ledy+15, 0])
            led3();
        translate([ledx+3*15, ledy+15, 0])
            led3();
        
        translate([25, 10+10, 0])
            led3();
    }
    
//    translate([edge, zmax-4*thick-edge, -thick])
//        cube([xmax-2*edge-4*thick, 3, 3]);

    translate([edge, 32, -thick])
        cube([xmax-2*edge-4*thick, 3, 3]);

    translate([edge, 6, -thick])
        cube([xmax-2*edge-4*thick, 3, 3]);

}

module led3() {
    linear_extrude(thick)
        circle(1.6); 
}

module led5() {
    linear_extrude(thick)
        circle(2.6); 
}

module floppy() {
    linear_extrude(thick)
        square([102 + 0.1, 26 + 0.1]);
}

module reset() {
    linear_extrude(thick)
        circle(5.5); 
}

module switch() {
    linear_extrude(thick)
        square([15 + 0.3, 21 + 0.3]);
}

module base_shape() {
    
    rotate([0,0,0])
    linear_extrude(1.6)
        base_shape_2d(thick * tf);

}

module base_shape_2d(th) {
    
    echo("width:", xmax-2*th);
    echo("height:", zmax-2*th);
    
    polygon([[0, zmax-edge-2*th], [0, edge], [edge,0], [xmax-edge-2*th, 0], [xmax-2*th, edge], [xmax-2*th, zmax-edge-2*th],[xmax-edge-2*th, zmax-2*th], [edge, zmax-2*th]  ]);
}

module beschriftung() {
    
    translate([49,-29.5, 0])
        cube([25, 0.6, 0.2]);
    translate([49,-11.75, 0])
        cube([25, 0.6, 0.2]);
    
    linear_extrude(0.2){
    translate([127, -25.5, -2 ])
    rotate([180, 0, 0])
    {
        text("Drive        A           B           C          D ", 3
    , "Arial:style=Bold"  );
    }
    
    translate([145, -10, -2 ])
    rotate([180, 0, 0])
    {
        text("DD        FM        Side B", 3
    , "Arial:style=Bold"  );
    }

    translate([210, -16, -2 ])
    rotate([180, 0, 0])
    {
        text("Reset", 3
    , "Arial:style=Bold"  );
    }
    
    translate([22, -28, -2 ])
    rotate([180, 0, 0])
    {
        text("ON -", 3
    , "Arial:style=Bold"  );
    }

    translate([22.25, -10.5, -2 ])
    rotate([180, 0, 0])
    {
        text("Off -", 3
    , "Arial:style=Bold"  );
    }
    
    translate([33, -5, -2 ])
    rotate([180, 0, 0])
    {
        text("Power                                 A             B", 3
    , "Arial:style=Bold"  );
    }
}

}
