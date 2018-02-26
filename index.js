/*
 * Copyright 2018, mike at github.com/iskinmike
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

define([
    "assert",
    "wilton/loader",
    "wilton/process"
], function(assert, loader, process) {
    "use strict";

    print("test: wilton/process");

    var executable =  loader.findModulePath("");
    print(executable);
    var ex_path = executable.replace("../std.wlib/",""); // remove std.wlib if it exists
    print(ex_path);
    var ex_full_path = ex_path + "fingerprint_for_wilton"; // wilton bin and fingerprint_for_wilton at the same directory
    print(ex_full_path);

    var code = process.spawn({
        executable: ex_full_path, 
        args: ["-f", "image.png", "-t", "5000"], 
        outputFile: "fingerprint_out.txt",
        awaitExit: true
    });
    print(code);
    
});