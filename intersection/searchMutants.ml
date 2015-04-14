open Printf

let (|>) v f = f v

(*
 * global variable to count a total mutants by read file
 *)
let totalMutants = begin ref 0 end

(*
 *@in a string s
 *@out a list with caracters of s
 *)
let division s = begin
  let rec exp i l =
    if i < 0 then l else exp (i - 1) (s.[i] :: l) in
  List.filter (fun x -> x != ' ') (exp (String.length s - 1) [])
  |> List.map (fun x -> (int_of_char x) -48)
end (*transforming char in int*)

(*
 *@in a int number and the file of write
 *@out a file writed with a int number 
 *)
let write number out= begin
  let oc = open_out_gen [Open_creat; Open_text; Open_append] 0o640 out in
  output_string oc (string_of_int number ^ " ");
  close_out oc
end

(*
 *@in a string and the file of write
 *@out a file writed with the string
 *)
let writeSession name out = begin
  let oc = open_out_gen [Open_trunc; Open_creat; Open_text; Open_append] 0o640 out in
  output_string oc (name);
  close_out oc
end

(*
 *@in a string and a filename
 *@out a file writed with a whole mutants alives by positive test case execution
 *)
let processAlives name filename = begin
  let in_channel = open_in filename in
  let count = ref 0 in (*to count the line of mutant*)
    try
      while true do
        let line = input_line in_channel in
        let l = division line in
        
        if ((not (List.mem 2 l) && not (List.mem 3 l) && not (List.mem 4 l) && not (List.mem 5 l) && not (List.mem 7 l) )) 
        	then write !count ("AlivesAndKilleds/" ^ name ^ "_AlivesPositives.trc");
        
        count:= !count + 1;
      done
    with End_of_file -> close_in in_channel
end

(*
 *@in a string and a filename
 *@out a file writed with a whole mutants killed by negative test case execution
 *)
let processKilleds name filename = begin
  let in_channel = open_in filename in
  let count = ref 0 in (*to count the line of mutant*)
    try
      while true do
        let line = input_line in_channel in
        totalMutants:= !totalMutants + 1;
        let l = division line in

        if (not (List.mem 1 l) && not (List.mem 6 l) && not (List.mem 7 l))
        	then write !count ("AlivesAndKilleds/" ^ name ^ "_KilledNegatives.trc");
        
        count:= !count + 1;
      done
    with End_of_file -> close_in in_channel
end

let () = 
  
  writeSession Sys.argv.(1) ("results/sessionName.trc");
  
  processAlives Sys.argv.(1) Sys.argv.(2);
  writeSession ("AlivesAndKilleds/" ^ Sys.argv.(1) ^ "_AlivesPositives.trc") ("results/positiveReport.trc");

  processKilleds Sys.argv.(1) Sys.argv.(3);
  writeSession ("AlivesAndKilleds/" ^ Sys.argv.(1) ^ "_KilledNegatives.trc") ("results/negativeReport.trc");

  writeSession (string_of_int !totalMutants) ("results/totalMutants.trc");

()