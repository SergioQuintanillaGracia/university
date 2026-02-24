#!/usr/bin/env python
#! -*- encoding: utf8 -*-

# 1.- Pig Latin

import re
import sys
from typing import Optional, Text
from os.path import isfile

class Translator():

    def __init__(self, punt:Optional[Text]=None):
        """
        Constructor de la clase Translator

        :param punt(opcional): una cadena con los signos de puntuación
                                que se deben respetar
        :return: el objeto de tipo Translator
        """
        if punt is None:
            punt = ".,;?!"
        self.re = re.compile(r"(\w+)([" + punt + r"]*)")

    def translate_word(self, word:Text) -> Text:
        """
        Recibe una palabra en inglés y la traduce a Pig Latin

        :param word: la palabra que se debe pasar a Pig Latin
        :return: la palabra traducida
        """

        punctuations = ('.', ',', ';', '?', '!')
        
        # Store the punctuation character at the end of the word if it exists
        punctuation = ''
        if word[-1] in punctuations:
            punctuation = word[-1]
            word = word[:-1]

        # If the word contains numbers or characters that are not letters, don't
        # process it
        if not word.isalpha():
            return word + punctuation

        vowels = ('y', 'a', 'e', 'i', 'o', 'u')
        is_first_uppercase = word[0].isupper()
        is_all_uppercase = is_first_uppercase and word[-1].isupper()

        # We process the word in uppercase to simplify the process
        word = word.lower()

        if word[0] in vowels:
            new_word = word + 'yay' + punctuation

        else:
            # Find the first index of a vowel
            first_vowel_i = 0
            for i in range(len(word)):  # We could also use enumerate
                if word[i] in vowels:
                    first_vowel_i = i
                    break
            
            new_word = word[first_vowel_i:] + word[:first_vowel_i] + 'ay' + punctuation
        
        # Restore capitalization
        if is_all_uppercase:
            new_word = new_word.upper()

        elif is_first_uppercase:
            new_word = new_word.capitalize()

        return new_word

    def translate_sentence(self, sentence:Text) -> Text:
        """
        Recibe una frase en inglés y la traduce a Pig Latin

        :param sentence: la frase que se debe pasar a Pig Latin
        :return: la frase traducida
        """

        new_sentence_arr = [self.translate_word(word) for word in sentence.split(' ')]
        new_sentence = ' '.join(new_sentence_arr)

        return new_sentence

    def translate_file(self, filename:Text):
        """
        Recibe un fichero y crea otro con su tradución a Pig Latin

        :param filename: el nombre del fichero que se debe traducir
        :return: None
        """
        
        if not isfile(filename):
            print(f'{filename} no existe o no es un nombre de fichero', file=sys.stderr)

        with open(filename, 'r') as original:
            filename_arr = filename.split('.')
            new_filename = filename_arr[0] + '_latin' + '.' + filename_arr[1]

            # Empty the file in case it already exists
            open(new_filename, 'w').close()

            with open(new_filename, 'a') as newfile:
                for line in original:
                    newfile.write(self.translate_sentence(line.strip()) + '\n')

if __name__ == "__main__":
    if len(sys.argv) > 2:
        print(f'Syntax: python {sys.argv[0]} [filename]')
        exit()
    t = Translator()
    if len(sys.argv) == 2:
        t.translate_file(sys.argv[1])
    else:
        sentence = input("ENGLISH: ")
        while len(sentence) > 1:
            print("PIG LATIN:", t.translate_sentence(sentence))
            sentence = input("ENGLISH: ")
