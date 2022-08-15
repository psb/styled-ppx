/* esy x lexer-renderer */

let render_help = () => {
  print_endline("");
  print_endline("");
  print_endline(
    {|  lexer-renderer pretty-prints the lexer of CSS|},
  );
  print_endline("");
  print_endline({|    EXAMPLE: esy x lexer-renderer ".a { color: red }"|});
  print_endline("");
  print_endline("");
};

let container_lnum = 0;
let pos = Lexing.dummy_pos;
let args = Sys.argv |> Array.to_list;
let input = List.nth_opt(args, 1);
let help =
  List.exists(
    arg =>
      arg == "--help"
      || arg == "-help"
      || arg == "help"
      || arg == "--h"
      || arg == "-h",
    args,
  );

let position_to_string = pos =>
  Printf.sprintf(
    "[%d,%d+%d]",
    pos.Lexing.pos_lnum,
    pos.Lexing.pos_bol,
    pos.Lexing.pos_cnum - pos.Lexing.pos_bol,
  );

let location_to_string = loc =>
  Printf.sprintf(
    "%s..%s",
    position_to_string(loc.Location.loc_start),
    position_to_string(loc.Location.loc_end),
  );

let rec printUnlessIsEof = buffer => {
  let (lexes, _loc_start, _loc_end) = Css_lexer.get_next_tokens_with_location(buffer);
  lexes |> Css_lexer.token_to_debug |> print_endline;
  /* let position = position_to_string(position_end);
  let position_at = position_to_string(position_end_at);
  print_endline(token_to_debug(token) ++ {| [|} ++ position ++ {|..|} ++ position_at ++ {|]|}); */

  switch (lexes) {
    | Css_lexer.Parser.EOF => ()
    | _token => printUnlessIsEof(buffer)
  }
};

switch (input, help) {
| (Some(_), true)
| (None, _) => render_help()
| (Some(css), _) => css |> Lex_buffer.from_string |> printUnlessIsEof;
};
