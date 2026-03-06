#!/usr/bin/env python
#! -*- encoding: utf8 -*-
# 3.- Mono Info

import argparse
from SAR_p3_monkey_lib import Monkey


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Show the information of certain language model.')
    parser.add_argument('lm_filename', type=str, action='store',
                        help='language model file.')   
    parser.add_argument('-i', '-info', dest='info_filename', type=str, action='store', default=None,
                        help='name of the file to save the language model information')

    args = parser.parse_args()
    m = Monkey()
    m.load_lm(args.lm_filename)
    if args.info_filename:    
        m.save_info(args.info_filename)
    else:
        m.show_info()
