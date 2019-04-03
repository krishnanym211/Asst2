//    //boolean to check if Command is Recursive
 //    int isRec = 0;
 //    if (strcmp("-R",argv[1]) == 0 || strcmp("-R",argv[2]) == 0) {
 //        isRec = 1;
 //    }
 //    //Oper reads the flag and determines which operation to conduct
 //    char Oper = ' ';
 //    if (isRec) {
 //        if (strcmp("-R", argv[1]) == 0) {
 //            Oper = argv[2][1];
 //        } else {
 //            Oper = argv[1][1];
 //        }
 //    } else {
 //        Oper = argv[1][1];
 //    }
    
 //    //min# of input = 3 &max# of input = 5
 //    //look for argument of File or Directory
 //    char * input = argv[3 + (isRec - 1)];
    
 //    //check if input is a File or Directory
 //    int isFile = 1;
 //    struct stat pstat;
 //    if (stat(input, &pstat) != 0) {
 //        printf("ERROR: Directory or file \"%s\" does not exist. Please try again.\n", input);
 //        return EXIT_FAILURE;
 //    }
 //    // Check if user inputted a directory or a file
 //    isFile = S_ISREG(pstat.st_mode);
    
 //    // Get rid of trailing '/' if present
 //    if (input[strlen(input) - 1] == '/') {
 //        input[strlen(input) - 1] = '\0';
 //    }
    
 //    //Check for invalid command
 //    if (Oper != 'b' && Oper != 'c' && Oper != 'd') {
 //        printf("ERROR: Invalid flag. Please refer to Readme for valid flags and thier definitions.\n");
 //        return EXIT_FAILURE;
 //    }
    
 //    //Conditional to Build Codebook
	// if(isRec && Oper == 'b'){
	// 	directoryHandler(argv[2], "index");
	// }
	// else if(Oper == 'b'){
	// 	fileHandler(argv[2], "index");
	// }
    
 //    //if not recursive and Operations Compress or Decompress are called, look for codebook and continue
 //    if(isFile && (Oper == 'c' || Oper == 'd')){
 //        int fd_file = open(input, O_RDONLY);
 //        if (fd_file < 0) {
 //            printf("ERROR: Could not open file \"%s\". Please try again.\n", input);
 //            return EXIT_FAILURE;
 //        }
 //        //Check for valid number of inputs considering command line needs exactly 4 inputs (exec, flag, file/dir , codebook)
 //        if (argc > (5 + isRec - 1)) {
 //            printf("ERROR: Excessive arguments.\n");
 //            return EXIT_FAILURE;
 //        } else if (argc < (5 + isRec - 1)) {
 //            printf("ERROR: Insufficient arguments.\n");
 //            return EXIT_FAILURE;
 //        }
 //        // Check to ensure HuffmanCodebook file was provided */
 //        if (strlen(argv[argc - 1]) < strlen("HuffmanCodebook")) {
 //            printf("ERROR: HuffmanCodebook file required.\n");
 //            return EXIT_FAILURE;
 //        }
 //        char * test_codebook = argv[argc - 1] + (strlen(argv[argc - 1]) - 15);
 //        if (strcmp(test_codebook, "HuffmanCodebook") != 0) {
 //            printf("ERROR: HuffmanCodebook file required.\n");
 //            return EXIT_FAILURE;
 //        }
 //        int fd_codebook = open(argv[argc - 1], O_RDONLY);
 //        if (fd_codebook < 0) {
 //            printf("Error: Could not open file \"%s\". Please try again.\n", argv[argc - 1]);
 //            return EXIT_FAILURE;
 //        }
 //        //now you have the file and the codebook to work with. Check the flag and call appropriate function.
 //        if(Oper == 'c'){
 //            //Compress file
 //            // Create new file name with .hcz extension
 //            char * hczfile = strcat(input, ".hcz");
 //            int fd_hcz = open(hczfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
 //            if (fd_hcz < 1) {
 //                printf("ERROR: Could not open or create file \"%s\".\n", hczfile);
 //                return EXIT_FAILURE;
 //            }
 //            //call compress with new file, input file and codebook
 //            compress(fd_hcz, fd_file, fd_codebook);
 //            close(fd_hcz);
            
 //        }else{
 //            //Decompress file
 //            char * test = (char *)malloc(sizeof(char) * 5);
 //            test = input + strlen(input) - 4;
 //            // Ensure that file is an .hcz file
 //            if (strcmp(test, ".hcz") != 0) {
 //                printf("ERROR: Decoding requires a .hcz file.\n");
 //                return EXIT_FAILURE;
 //            }
 //            // Cut off last four chars to get rid of .hcz and make new file
 //            char * result = input;
 //            result[strlen(input) - 4] = '\0';
 //            int fd_res = open(result, O_WRONLY | O_CREAT , 0644);
 //            if (fd_res < 1) {
 //                printf("ERROR: Could not open or create file \"%s\".\n", result);
 //                return EXIT_FAILURE;
 //            }
 //            //call compress with results fle, compressed file and codebook
 //            decompress(fd_res, fd_file, fd_codebook);
 //            close(fd_res);
 //        }
 //        close(fd_codebook);
 //    }