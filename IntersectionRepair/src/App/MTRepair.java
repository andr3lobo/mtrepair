/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package App;

import gadgets.IntroClassView;

/**
 *
 * @author andrelobo
 */
public class MTRepair {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {

        IntroClassView view = new IntroClassView("/home/andrelobo/NetBeansProjects/mtrepair/IntroClass");
        view.loadBenchNames();
        view.loadDirMap();
    }
    
}
