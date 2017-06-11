#!/usr/bin/python
import random, sys, locale
from optparse import OptionParser


class comm:
    def __init__(self, args, file1, file2, both, unsorted):
        if args[0] == '-':
            self.lines_f1 = sys.stdin.readlines()
        else:
            f1 = open(args[0], 'r')
            self.lines_f1 = f1.readlines()
            f1.close()

        if args[1] == '-':
            self.lines_f2 = sys.stdin.readlines()
        else:
            f2 = open(args[1], 'r')
            self.lines_f2 = f2.readlines()
            f2.close()
        self.file1_sup = file1
        self.file2_sup = file2
        self.both_sup = both
        self.unsorted = unsorted
        self.lines_f1 = self.addNewLine(self.lines_f1)
        self.lines_f2 = self.addNewLine(self.lines_f2)
        self.filler = "\t"
        
    def doEmptyFiles(self):
        len1 = len(self.lines_f1)
        len2 = len(self.lines_f2)
        if len1 == 0 or len2 == 0:
            if len1:
                for line in self.lines_f1:
                    self.output(line, 1)
            elif len2:
                for line in self.lines_f2:
                    self.output(line, 2)
            return True
        return False


    #check if two files are sorted
    def issorted(self):
        file1_sorted = list(self.lines_f1)
        file1_sorted.sort()
        file2_sorted = list(self.lines_f2)
        file2_sorted.sort()
        sorted = True
        if self.lines_f1 != file1_sorted:
            sorted = False
            sys.stderr.write("FILE 1 is not in sorted order\n")
        if self.lines_f2 != file2_sorted:
            sorted = False
            sys.stderr.write("FILE 2 is not in sorted order\n")
        return sorted    
        
    #compare two (un)sorted files and generate three columns 
    def compare(self):
        #unsorted file 
        if self.unsorted:
            if self.doEmptyFiles():
                return 
            line2 = list(self.lines_f2)
            for line1 in self.lines_f1:
                found = False
                for line in line2:
                    if line1 == line:
                        found = True
                        line2.remove(line1)
                        self.output(line1, 3)
                        break
                if found == False:
                    self.output(line1, 1)
            
            for line in line2:
                self.output(line, 2)
            return

        #if the files are not sorted, just return
        if not self.issorted():
            return
        
        if self.doEmptyFiles():
            return 
        #comm sorted file     
        i=0
        j=0
        while i<len(self.lines_f1) or j<len(self.lines_f2):
            line1 = ""
            line2 = ""
            if i<len(self.lines_f1):
               line1 = self.lines_f1[i]
            if j<len(self.lines_f2):
               line2 = self.lines_f2[j]
            
            if (line1 == "" or (line2 != "" and line1>line2)):
                     self.output(line2, 2)
                     j+=1
            elif (line2 == "" or (line1 != "" and line1<line2)):
                     self.output(line1, 1)
                     i+=1
            else:
                  self.output(line1, 3)
                  i+=1
                  j+=1
                    
    #std output and suppress corresponding columns
    #line is the output text, state is which col that is suppresed
    def output(self, line, state):
        stream = ""
        if state == 1:
            if self.file1_sup == True:
                  return 
        elif state == 2:
            if self.file2_sup == True:
                  return
            if self.file1_sup == False:
                stream += self.filler
        elif state == 3:
            if self.both_sup == True:
                  return
            if self.file1_sup == False:
                  stream += self.filler
            if self.file2_sup == False:
                  stream += self.filler
        else:
            sys.stderr.write("suppress error")
        stream += line
        sys.stdout.write(stream)
    
    def addNewLine(self, file):
        if len(file)!=0:
            temp = file[-1]
            if '\n' not in temp:
                temp += '\n'
            file[-1] = temp
        return file

def main():
    locale.setlocale(locale.LC_ALL, 'C')
    version_msg = "prog 1.0"
    usage_msg = """prog [-123] FILE1 FILE2

select or reject lines common to two files"""

    parser = OptionParser(version = version_msg,
                          usage = usage_msg)
    parser.add_option("-1", action="store_true", dest="onlyFile1",
            default=0, help="Suppress the output column of lines unique to file1.") 
    parser.add_option("-2", action="store_true", dest="onlyFile2",
            default=0, help="Suppress the output column of lines unique to file2.") 
    parser.add_option("-3", action="store_true", dest="both",
            default=0, help="Suppress the output column of lines duplicated in file1 and file2") 
    parser.add_option("-u", action="store_true", dest="unsorted",
            default=0, help="causes comm to work even if its inputs are not sorted") 
    options, args = parser.parse_args(sys.argv[1:])

    #print(args)
    #print(options)
    #argument check
    if len(args)!=2:
        parser.error("invalid number of OPTION arguments")
       
    if args[0] == '-' and args[1] =='-':
        parser.error("only one FILE argument can be STDIN")

    #flag check
    try:
        file1 = bool(options.onlyFile1);
        file2 = bool(options.onlyFile2);
        both = bool(options.both);
        unsorted = bool(options.unsorted);
    except:
        parser.error("invalid OPTION type: {0}".
                     format(options.onlyFile1))
    
    try:
        generator = comm(args, file1, file2, both, unsorted)
    except IOError as err:
        parser.error("I/O error({0}):{1}".
                     format(err.errno, err.strerror))
    try:
        generator.compare()
    except:
       parser.error("Comparing Files Error!")

if __name__ == "__main__":
    main()
