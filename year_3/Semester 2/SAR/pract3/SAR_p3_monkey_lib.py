#!/usr/bin/env python
#! -*- encoding: utf8 -*-
# 3.- Mono Library

import pickle
import random
import re
import sys
from typing import List, Optional, TextIO

## Nombres: 

########################################################################
########################################################################
###                                                                  ###
###  Todos los métodos y funciones que se añadan deben documentarse  ###
###                                                                  ###
########################################################################
########################################################################


def convert_to_lm_dict(d: dict):
    for k in d:
        l = sorted(((y, x) for x, y in d[k].items()), reverse=True)
        d[k] = (sum(x for x, _ in l), l)


class Monkey():

    def __init__(self):
        self.r1 = re.compile('[.;?!]')
        self.r2 = re.compile('\W+')
        self.info = {}

    def get_n(self):
        return self.info.get('n', 0)

    def index_sentence(self, sentence:str):
        n = self.info['n']
        #############
        # COMPLETAR #
        #############
        
        sentence_arr = sentence.split()
        
        sentence_arr.insert(0, '$')
        sentence_arr.append('$')
        
        for curr_n in range(2, n + 1):
            for i in range(len(sentence_arr)):
                curr_words = sentence_arr[i : i + curr_n]
                dict_entry = self.info['lm'][curr_n][curr_words]
                dict_entry = dict_entry.get(curr_words, 0) + 1
            
            sentence_arr.insert(0, '$')

    def compute_lm(self, filenames:List[str], lm_name:str, n:int):
        self.info = {'name': lm_name, 'filenames': filenames, 'n': n, 'lm': {}}
        for i in range(2, n+1):
            self.info['lm'][i] = {}
        for filename in filenames:
            with open(filename, encoding='utf-8') as fh:
                for line in fh:
                    #############
                    # COMPLETAR #
                    #############
                    processed_line = self.r2.sub(' ', line.strip().lower())
                    
                    sentences = self.r1.split(processed_line)
                    
                    for sentence in sentences:
                        self.index_sentence(sentence)
                
        for i in range(2, n+1):
            convert_to_lm_dict(self.info['lm'][i])

    def load_lm(self, filename:str):
        with open(filename, "rb") as fh:
            self.info = pickle.load(fh)

    def save_lm(self, filename:str):
        with open(filename, "wb") as fh:
            pickle.dump(self.info, fh)

    def save_info(self, filename:str):
        with open(filename, "w", encoding='utf-8', newline='\n') as fh:
            self.print_info(fh=fh)

    def show_info(self):
        self.print_info(fh=sys.stdout)

    def print_info(self, fh:TextIO):
        print("#" * 20, file=fh)
        print("#" + "INFO".center(18) + "#", file=fh)
        print("#" * 20, file=fh)
        print(f"language model name: {self.info['name']}", file=fh)
        print(f'filenames used to learn the language model: {self.info["filenames"]}', file=fh)
        print("#" * 20, file=fh)
        print(file=fh)
        for i in range(2, self.info['n']+1):
            print("#" * 20, file=fh)
            print("#" + f'{i}-GRAMS'.center(18) + "#", file=fh)
            print("#" * 20, file=fh)
            for prev in sorted(self.info['lm'][i].keys()):
                wl = self.info['lm'][i][prev]
                print(f"'{' '.join(prev)}'\t=>\t{wl[0]}\t=>\t{', '.join(['%s:%s' % (x[1], x[0]) for x in wl[1]])}" , file=fh)


    def generate_sentences(self, n:Optional[int], nsentences:int=10, prefix:Optional[str]=None):
        #############
        # COMPLETAR #
        #############
        pass


if __name__ == "__main__":
    print("Este fichero es una librería, no se puede ejecutar directamente")
