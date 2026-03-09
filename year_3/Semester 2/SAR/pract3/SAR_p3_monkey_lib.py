#!/usr/bin/env python
#! -*- encoding: utf8 -*-
# 3.- Mono Library

import pickle
import random
import re
import sys
from typing import List, Optional, TextIO

## Nombres: 
# Antonio Gasperini
# Sergio Quintanilla Graciá

########################################################################
########################################################################
###                                                                  ###
###  Todos los métodos y funciones que se añadan deben documentarse  ###
###                                                                  ###
########################################################################
########################################################################

MAX_SENTENCE_WORDS = 50


def convert_to_lm_dict(d: dict):
    for k in d:
        l = sorted(((y, x) for x, y in d[k].items()), reverse=True)
        d[k] = (sum(x for x, _ in l), l)


class Monkey():

    def __init__(self):
        self.r1 = re.compile('[.;?!]')
        self.r2 = re.compile(r'\W+')
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
            for i in range(len(sentence_arr) - curr_n + 1):
                curr_words = tuple(sentence_arr[i : i + curr_n])
                first_words = curr_words[:-1]
                last_word = curr_words[-1]
                
                n_dict = self.info['lm'][curr_n]
                # Initialize the dictionary entry corresponding to the first words
                n_dict[first_words] = n_dict.get(first_words, {})
                    
                n_dict[first_words][last_word] = n_dict[first_words].get(last_word, 0) + 1
            
            sentence_arr.insert(0, '$')

    def compute_lm(self, filenames:List[str], lm_name:str, n:int):
        self.info = {'name': lm_name, 'filenames': filenames, 'n': n, 'lm': {}}
        for i in range(2, n+1):
            self.info['lm'][i] = {}
        for filename in filenames:
            with open(filename, encoding='utf-8') as fh:
                #############
                # COMPLETAR #
                #############
                text = fh.read()
                text = text.replace('\n\n', '. ')
                sentences = self.r1.split(text)
                processed_setences = [self.r2.sub(' ', sentence.lower()).strip() for sentence in sentences]
                
                for sentence in processed_setences:
                    if not sentence.strip():
                        continue
                    
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
        if not n:
            n = self.info['n']
        
        sentence = '' if prefix is None else prefix
                
        # Currently, the sentence contains just the prefix
        # Format it
        sentence = self.r2.sub(' ', sentence.lower()).strip()
        
        # Initialize leading $
        sentence = '$ ' * (n - 1) + sentence
        
        initial_sentence_arr = [word.strip() for word in sentence.split() if word.strip()]
        
        for i in range(nsentences):
            sentence_arr = initial_sentence_arr.copy()
            
            next_word = None
            word_count = 0
            
            while next_word != '$' and word_count <= MAX_SENTENCE_WORDS:
                prev_words = tuple(sentence_arr[-n + 1:])
                # Contains data in the structure (11, [(5, ’spam’), (5, ’egg’), (1, ’lobster’)])
                next_words_info = self.info['lm'][n][prev_words]
                weight_sum = next_words_info[0]
                possible_next_words = next_words_info[1]
                
                random_weight = random.random()  # Random weight in [0, 1)
                # We will increase weight_progress from 0 to 1 word by word, until it exceeds the
                # random value calculated
                # When we get to that value, we select that word
                weight_progress = 0
                
                for (weight, word) in possible_next_words:
                    weight_increase = weight / weight_sum
                    weight_progress += weight_increase
                    
                    if weight_progress >= random_weight:
                        # We select the current word
                        next_word = word
                        break
                
                sentence_arr.append(next_word)
                word_count += 1
            
            sentence = ' '.join(sentence_arr).replace('$', '').strip()
            
            print(sentence)


if __name__ == "__main__":
    print("Este fichero es una librería, no se puede ejecutar directamente")
