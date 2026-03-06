#!/usr/bin/env python
#! -*- encoding: utf8 -*-
# 3.- Mono Indexer

import argparse
from SAR_p3_monkey_lib import Monkey


if __name__ == "__main__":


    parser = argparse.ArgumentParser(description='Compute a n-gram language model from text files.')
    parser.add_argument('files', metavar='files', type=str, nargs='+',
                        help='text files.')

    parser.add_argument('-l', '--lm_name', dest='lm_name', action='store', required=False, default=None,
                        help='name of the language model.')

    parser.add_argument('-f', '--filename', dest='lm_filename', action='store', required=True,
                        help='name of the file to save the language model.')
    
    parser.add_argument('-n', action='store', type=int, required=False, default=3,
                        help='maximum value of n for n-grams.')

    args = parser.parse_args()
    m = Monkey()
    m.compute_lm(args.files, args.lm_name, args.n)
    m.save_lm(args.lm_filename)
    
