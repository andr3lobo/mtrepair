open Str

let (|>) v f = f v

module Int_set =
  Set.Make (struct
    type t = int
    let compare = compare
  end)

(*
 * @in string with numbers separated by spaces
 * @out int list
 *)
let explode s = begin
  List.map int_of_string (Str.split (Str.regexp " ") s)
end

(*
 * @in filename
 * @out int list
 * @obs it reads only one line of the file
 *)
let read_lines filename : int list = begin
  let ic = open_in filename in (* ic = input channel *)
  let try_read() =
    try Some(input_line ic) with End_of_file -> None
  in

  match try_read () with
  | Some s -> explode s
  | None -> close_in ic; []
end

(*
 * @in int list
 * @out int set
 *)
let set_of_list = begin
  List.fold_left (fun acc x -> Int_set.add x acc) Int_set.empty
end

(*
 * @in filename
 * @out int set
 *)
let process_data filename = begin
  filename
  |> read_lines
  |> set_of_list
end

(*
 * @in a list t
 * @out the size of t
 *)
let length list = begin
    let rec aux n = function
      | [] -> n
      | _::t -> aux (n+1) t
    in aux 0 list
end

(*
 * @in a string (a path of file)
 * @out a string (read from file)
 *)
let readInfSession filename = begin
  let ic = open_in filename in (* ic = input channel *)
  let try_read() = (try Some(input_line ic) with End_of_file -> None) in
  match try_read () with
  | Some s -> s
  | None -> close_in ic; ""
end

(*
 * @in a int number x and path to write in the file
 * @out the file writed with x
 *)
let write number out= begin
  let oc = open_out_gen [Open_creat; Open_text; Open_append] 0o640 out in
  output_string oc (string_of_int number ^ " ");
  close_out oc
end

(*
 * @in string str and path to write in the file
 * @out the file writed with str
 *)
let writeMetrics result out = begin
  let oc = open_out_gen [Open_creat; Open_text; Open_append] 0o640 out in
  output_string oc (result);
  close_out oc
end

let () =
  let sessionName = readInfSession "results/sessionName.trc" in
  let totalMutants = readInfSession "results/totalMutants.trc" in

  let positiveReport = readInfSession "results/positiveReport.trc" in
  let negativeReport = readInfSession "results/negativeReport.trc" in

  let alives = process_data positiveReport in
  let killed = process_data negativeReport in
  let intersection = Int_set.inter alives killed in

  let sizeAlives = (string_of_int (length (Int_set.elements alives))) in
  let sizeKilled = (string_of_int (length (Int_set.elements killed))) in
  let sizeIntersection = (string_of_int (length (Int_set.elements intersection))) in

  Int_set.iter (fun x -> 
  	write (x) ("results/" ^ sessionName ^ "_intersectionMutants.trc");
  ) intersection;

  writeMetrics (sessionName ^ ";" ^ totalMutants ^ ";" ^ sizeAlives ^ ";" ^ sizeKilled ^ ";" ^ sizeIntersection ^ ";\n") ("results/results.csv");
  
  
  (*cleanning logs*)
  (*---------------------------------------*)
  Sys.remove ("intersectionMutants.cmi");
  Sys.remove ("intersectionMutants.cmo");
  Sys.remove ("a.out");
  Sys.remove ("results/sessionName.trc");
  Sys.remove ("results/totalMutants.trc");
  Sys.remove ("results/positiveReport.trc");
  Sys.remove ("results/negativeReport.trc");
  (*---------------------------------------*)
()
