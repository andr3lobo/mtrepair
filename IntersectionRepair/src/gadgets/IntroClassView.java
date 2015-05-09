/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package gadgets;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;

/**
 *
 * @author andrelobo
 */
public class IntroClassView {
    
    public List<String> benchNames;//The list with the benchmarks names 
    public String path;
    // what is the list of test
    //what are the negative test cases
    //what are the positive test cases
    
    /**
     * This hash has the directory tree of IntroClass benchmark
     * To browse in dirMap structure is essential for create
     * the test sessions, to choice the version to be investigated, etc
     * HashMap <benchName, HashMap <studentName, VersionsInf class>
     */
    public HashMap <String,HashMap <String, VersionsInf>> dirMap = new HashMap<>();
    
    /**
     * Constructing a view class from a given benchmark path
     * @param benchmarkPath 
     */
    public IntroClassView (String benchmarkPath){
        this.path = benchmarkPath;
        this.benchNames = new ArrayList<>();       
    }
    /**
     * Capturing the benchmarks names,
     * helps in creation of test sessions and 
     * to search c programs into IntroClass
     */
    public void loadBenchNames (){
        
        List<String> subPaths = getDirectories(this.path, "bin");
        
        for (String a: subPaths){
            String[] str = a.split("/");
            this.benchNames.add(str[str.length - 1]);
        }
        Collections.sort(benchNames);
    }
       
    /**
     * This method help to load the hashMap dirMap
     * @throws java.io.IOException
     */
    public void loadDirMap() throws IOException{
        
        for (String bchName : this.benchNames){
            List<String> studentNames = getDirectories(this.path + 
                    "/"+bchName,"tests");
            
            HashMap<String,VersionsInf> students = new HashMap<>();
            
            for (String stdName: studentNames){
                List<String> programVersions = getDirectories(stdName, "TestSessions");
                Collections.sort(programVersions);
                
                VersionsInf versions =  getVersions(programVersions);
                
                //if the result is null, not load the local HashMap
                if (versions != null){
                    students.put(stdName, versions);
                }
            }
            
            this.dirMap.put(bchName, students);
        }
    }
    
    /**
     * This class identifies if there is a correct version inside 
     * of the directories that contains the programs versions submitted 
     * by student (The rule is to capture a two versions: 
     * i)the correct version and ii) the immediately previous version )
     * @param programVersions is a list with directories names of the 
     *  specific student
     * @return the VersionsInf class
     */
    private VersionsInf getVersions(List<String> programVersions) throws IOException {
        
        VersionsInf verInf = new VersionsInf();//to verify correted version
        VersionsInf vInf = new VersionsInf();//to verify bugged version
        
        //indexes to locate last and previous versions
        int lastVersion = programVersions.size() - 1;
        int previousVersion = programVersions.size() - 2;
        
        List<String> negTCB
                = verInf.searchNegativeTC(programVersions.get(lastVersion), "blackbox");
        List<String> negTCW 
                = verInf.searchNegativeTC(programVersions.get(lastVersion),"whitebox");
        
        //if there is the possibility for some version corrected
        if (negTCB.isEmpty() || negTCW.isEmpty()){
            
            //if both version are correct in last version submission, verify in previous submission
            if (negTCB.isEmpty() && negTCW.isEmpty()){
                negTCB.addAll(verInf.searchNegativeTC(
                        programVersions.get(previousVersion), "blackbox"));
                
                negTCW.addAll(verInf.searchNegativeTC(
                        programVersions.get(previousVersion), "whitebox"));
                
                //if both has negative test case
                if (!negTCB.isEmpty() && !negTCW.isEmpty()){
                    vInf.setNtcB(negTCB);
                    vInf.setNtcW(negTCW);
                    
                //if negTCB is not empty
                } else if (!negTCB.isEmpty() && negTCW.isEmpty()){
                    vInf.setNtcB(negTCB);
                
                //if negTCW is not empty
                } else if (negTCB.isEmpty() && !negTCW.isEmpty()){
                    vInf.setNtcW(negTCW);
                }
              
              //if only negTCB is not empty, verify only whitebox test
            } else if(!negTCB.isEmpty() && negTCW.isEmpty()){
                negTCW.addAll(verInf.searchNegativeTC(
                        programVersions.get(previousVersion), "whitebox"));
                if (!negTCW.isEmpty())
                    vInf.setNtcW(negTCW);
              
              //if only negTCW is not empty, verify only blackbox test
            } else if (negTCB.isEmpty() && !negTCW.isEmpty()){
                negTCB.addAll(verInf.searchNegativeTC(
                        programVersions.get(previousVersion), "blackbox"));                
                if (!negTCB.isEmpty())
                    vInf.setNtcB(negTCB);            
            }
             
            //always the correct version will be the last
            vInf.setCorrectVersion(programVersions.get(lastVersion));
            //always the bugged version will be the previous
            vInf.setBuggedVersion(programVersions.get(previousVersion));            
            
            return vInf;
            
        } else {
            return null;
        }        
    }    
    
    /**
     * Getting a ArrayLits of the subPaths given a root path
     * @param rootPath root directory 
     * @param noDir subDirectory that not be include
     * @return a ArrayLits of the subPaths given a root path
     */
    public List<String> getDirectories (String rootPath, String noDir){
        
        File dir = new File(rootPath);
        File[] subDirs = dir.listFiles(File::isDirectory);
        List<String> subPaths = new ArrayList<>();
        List <String> paths = new ArrayList<>();
        
        for (File subdir: subDirs){
             paths.add(subdir.getPath());
            String[] str = paths.get(paths.size()-1).split("/");
            if (!noDir.equals(str[str.length - 1]))
                subPaths.add(subdir.getPath());
        }
        return subPaths;
    }
    
    /**
     * Method to obtain a list of all files given a directory
     * @param dir a directory path
     * @return a list of files inside of the path
     */
    public List<String> getFiles(String dir) {
        
        File folder = new File(dir);
        File[] listOfFiles = folder.listFiles();
        
        List<String> files = new ArrayList<>();
        for (File file : listOfFiles){
            files.add(file.getName());
        }
        
        return files;
    }    
    
    public void printList(List<?> list){
        for (Object elem : list) {
            System.out.println(elem+"  ");
        }
    }
    
    public void runMutations() throws IOException {
        createDirectoryStructure();
        executeMutations();
        
    }
    
    /**
         * 1 - create a directory structure.
         *  In this case, I should erase any directory if it there is.
         *  To follow:
            Structure: /inside path with
                     * versions submitted by student/ "given from my program" to create
                     * following directories structure:
                     * 
                     * TestSessions/
                     *
                     *      negativeTC/
                     *          "directory with the program copy" negative/ "directory
                     *          with the test cases copied by my java program"
                     * 
                     *      positiveTC/
                     *          "directory with the program copy" positive/ "directory
                     *          with the test cases copied by my java program"
                     *
                     *      intersection/
                     *          negative/ "directory with the test cases copied by my
                     *          java program"
                     
         * 2 - The copy files.
         *  I should to copy files:
         *      i) program that will be mutated (bugged version) to positive and 
         *         negative directory
         *      ii) poll of the test cases (negative or positive)
    */    
    public void createDirectoryStructure() throws IOException {
        /*//TO-DO*/
        for (String benchName : this.benchNames) {
            List<String> students = new ArrayList<>();
            
            //here, I get all students that have been submitted with benchmark bench
            students.addAll(this.dirMap.get(benchName).keySet());
            
            //root is submissions directory for a specific student
            for (String root: students){
                String t = root;
                root= root+"/TestSessions";
                
                if(new File(root).exists())
                    this.delete(new File(root));
                String positiveTC = root+"//positiveTC";
                String negativeTC = root+"//negative";
                String intersection = root+"//intersection";
                new File(root).mkdirs();
                new File(positiveTC).mkdirs();
                new File(negativeTC).mkdirs();
                new File(intersection).mkdirs();
                
                this.dirMap.get(benchName).get(t).printInformations();
                
            }
        }        
        
    }

/**
 * 1) Creating the test session with whole operators:
 * ../ProteumScripts/CreatingTestSession.pl <dir where test sessions is located> research <compiled name program> <test session name>
 *  <dir where test sessions is located>: in my case, is the directory created by me for each test session (positive, negative and intersection test sessions)
    *Ex.:* /inside path with versions submitted by student/TestSessions/positiveTC/
 *  <compiled name program>: in my case, is the benchmark name in my hash
 *  <compiled name program>: in my case, is the benchmark name in my hash
 *  <test session name>: in my case, it can has the name that I want
 * Obs.: <dir where test sessions is located> in this case, into directory <dir where test sessions is located> should have a directoty with the same name of the program under test (<compiled name program>). Inside of this directory should have a directory with the same name of test session name. Now, we are into a directory to execute the scripts that should:
 *      i) has a program under test (copy the program inside this directory
 *      ii) a directory with the pool of test case that will execute the mutants
            * Ex.:*
            *   positive (for /TestSessions/positiveTC/)
            *   negative (for /TestSessions/negativeTC/)
            *   negative (for /TestSessions/intersection/)
            *       in this case, will be a copy of the /TestSessions/negativeTC/

* 2) Importing and executing whole test cases:
* ../ProteumScripts/ImportingExecutingTestSet.pl <dir where test sessions is located> <directory name of the pool test cases> poke 1 x <compiled name program> <test session name>

*/    
    private void executeMutations() {
        /*//TO-DO*/
        /**
         * 1 - create test sessions and execute mutations
         */
    }
    
    void delete(File f) throws IOException {
        if (f.isDirectory()) {
            for (File c : f.listFiles()) {
                delete(c);
            }
        }
        if (!f.delete()) {
            throw new FileNotFoundException("Failed to delete file: " + f);
        }
    }    
    
}
