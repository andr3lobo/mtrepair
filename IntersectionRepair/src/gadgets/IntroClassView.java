/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package gadgets;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author andrelobo
 */
public class IntroClassView {
    
    public List<String> benchNames;//The list with the benchmarks names 
    public String path;
    
    /**
     * This hash has the directory tree of ItroClass benchmark
     * To browse in dirMap structure is essential for create
     * the test sessions, to choice the version to be investigated, etc
     * HashMap <benchName, HashMap <studentName, list of directory versions of student>
     */
    public HashMap <String,HashMap <String, List<String>>> dirMap = new HashMap<>();
    
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
     */
    public void loadDirMap(){
        
        for (String bchName : this.benchNames){
            List<String> studentNames = getDirectories(this.path + 
                    "/"+bchName,"tests");
            
            HashMap<String,List<String>> students = new HashMap<>();
            for (String stdName: studentNames){
                List<String> programVersions = getDirectories(stdName, "bla");
                Collections.sort(programVersions);
                
                List<String> versions = getVersions(programVersions);
                if (versions.size()>0)
                    students.put(stdName, versions);
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
     * @return one string list with two elements: i) a bug version path and
     *  ii) a correct version path
     */
    private List<String> getVersions(List<String> programVersions) {
        List<String> versions = new ArrayList<>();
        
        for (int i=0;i<programVersions.size()-1;i++){
            
            List<String> files1 = getFiles(programVersions.get(i));
            List<String> files2 = getFiles(programVersions.get(i+1));

            if (files1.contains("gp-001.log") && !files2.contains("gp-001.log")) {
                versions.add(programVersions.get(i));
                versions.add(programVersions.get(i+1));
                break;
            }
        }
        
        return versions;
        
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
        for (Iterator<?> it = list.iterator(); it.hasNext();) {
            Object elem = it.next();
            System.out.println(elem+"  ");
        }
}


}
