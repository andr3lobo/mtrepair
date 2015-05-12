/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package gadgets;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 *
 * @author andrelobo
 */
public class VersionsInf {
    
    public String correctVersion;//directory path to correct version
    public String buggedVersion;//diretory path to bugged version
    public String student;
    public String benchName;
    public String testType;
    List<String> ntcW = new ArrayList<>();//list with name of negative test case white box
    List<String> ntcB = new ArrayList<>();//list with name of positive test case black box
    
    public VersionsInf(){};
    
    public List<String> searchNegativeTC(String path, String testType) throws FileNotFoundException, IOException{
        
        BufferedReader buffWhite = new BufferedReader(new FileReader(path+"/"+testType+"_test.sh"));
        String line = "";
        List<String> negTC = new ArrayList<>();
        while (buffWhite.ready()) {
            line = buffWhite.readLine();
            Pattern p = Pattern.compile("n\\d\\)");
            Matcher m = p.matcher(line);
            if (m.find()){
                p = Pattern.compile("\\d+\\.out");
                m = p.matcher(line);
                if (m.find()){
                    String ntc = line.substring(line.indexOf(testType+"/") + 9,
                            line.lastIndexOf(".out"));
                    if (!negTC.contains(ntc))
                        negTC.add(ntc+".in");
                }
            }            
        }
        return negTC;
    }
    
    void printInformations() {
        System.out.println(""
                + "--------------------------------------------------\n"
                + "benchmark: "+this.benchName+"\n"
                + "correct version: " + this.getCorrectVersion() + "\n"
                + "bugged version: " + this.getBuggedVersion() + "\n"
                + "negativeTestCasesBlackBox: " + this.getNtcB() + "\n"
                + "negativeTestCaseWhiteBox: " + this.getNtcW() + "\n"
                + "test type: "+this.testType
        );        
    }
    
    public String getCorrectVersion() {
        return correctVersion;
    }

    public void setCorrectVersion(String correctVersion) {
        this.correctVersion = correctVersion;
    }

    public String getBuggedVersion() {
        return buggedVersion;
    }

    public void setBuggedVersion(String buggedVersion) {
        this.buggedVersion = buggedVersion;
    }

    public List<String> getNtcW() {
        return ntcW;
    }

    public void setNtcW(List<String> ntcW) {
        this.ntcW = ntcW;
    }

    public List<String> getNtcB() {
        return ntcB;
    }

    public void setNtcB(List<String> ntcB) {
        this.ntcB = ntcB;
    }

    public String getStudent() {
        return student;
    }

    public void setStudent(String student) {
        this.student = student;
    }

    public String getTestType() {
        return testType;
    }

    public void setTestType() {
        if (!this.ntcB.isEmpty() && !this.ntcW.isEmpty()){
            this.testType="both";
        } else if (!this.ntcB.isEmpty() && this.ntcW.isEmpty()){
            this.testType="blackbox";
        } else if (this.ntcB.isEmpty() && !this.ntcW.isEmpty()){
            this.testType="whitebox";
        }
    }

    public String getBenchName() {
        return benchName;
    }

    public void setBenchName(String benchName) {
        this.benchName = benchName;
    }   
    
}