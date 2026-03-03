#! -*- encoding: utf8 -*-

## Nombres: Antonio Gasperini, Sergio Quintanilla

########################################################################
########################################################################
###                                                                  ###
###  Todos los mÃ©todos y funciones que se aÃ±adan deben documentarse  ###
###                                                                  ###
########################################################################
########################################################################

import argparse
import re
import sys
import math
import json

def sort_dic_by_values(d, asc=True):
    return sorted(d.items(), key=lambda a: (-a[1], a[0]))

def get_ngrams(l, n, add_marks=False):
    if add_marks:
        l.append('$')
        l.insert(0, '$')
    ngrams = []
    for i in range(len(l)-n+1):
        ngrams.append(l[i:i+n])
    return ngrams

class WordCounter:

    def __init__(self):
        """
           Constructor de la clase WordCounter
        """
        self.clean_re = re.compile(r'\W+')

    def get_shannon_entropy(self, symbols: dict, total_symbols):
        entropy = 0

        for symbol, count in symbols.items():
            p = count / total_symbols
            entropy -= p * math.log2(p)
        
        return entropy

    def get_redundancy(self, shannon_entropy, num_of_unique_symbols):
        return 1 - shannon_entropy / math.log2(num_of_unique_symbols)
    
    def write_stats_text(
        self,
        filename,
        stats,
        use_stopwords,
        words_without_stopwords_count,
        vocab_size,
        total_symbol_count,
        unique_symbol_count,
        entropy,
        redundancy,
        top_words,
        alphabetical_words,
        top_symbols,
        alphabetical_symbols,
        top_word_pairs,
        alphabetical_word_pairs,
        top_symbol_pairs,
        alphabetical_symbol_pairs
    ):
        """
        Este mÃ©todo escribe en texto plano las estadísticas de un fichero
            
        :param 
            filename: el nombre del fichero destino.
            source_file: el nombre del fichero fuente.
            stats: las estadíticas del texto.
            use_stopwords: booleano, si se han utilizado stopwords
            words_without_stopwords_count: numero palabras sin las stopwords
            vocab_size: numero de diferentes palabras
            total_symbol_count: numero total de caracteres
            unique_symbol_count: numero de caracteres diferentes
            entropy: valor de entropy calculado
            redundancy: valor de redundancia calculado
            top_words: palabras mas frecuentes
            top_symbols: simbolos mas frecuentes
            top_word_pairs: bigrams mas frecuentes en frases
            top_symbol_pairs: bigrams mas frecuentes en simbolos
    
        :return: None
        """

        with open(filename, "w", encoding="utf-8") as fh:

            # Basic stats
            fh.write(f"Lines: {stats['nlines']}\n")
            fh.write(f"Number of words (including stopwords): {stats['nwords']}\n")

            if use_stopwords:
                fh.write(
                    f"Number of words (without stopwords): "
                    f"{words_without_stopwords_count}\n"
                )

            fh.write(f"Vocabulary size: {vocab_size}\n")
            fh.write(f"Number of symbols: {total_symbol_count}\n")
            fh.write(f"Number of different symbols: {unique_symbol_count}\n")

            # Entropy
            if entropy is not None:
                fh.write(f"Shannon entropy: {entropy:.4f} bits/symbol\n")

            if redundancy is not None:
                fh.write(f"Redundancy: {redundancy * 100:.2f}%\n")

            # Words alphabetical
            fh.write("Words (alphabetical order):\n")
            for word in alphabetical_words:
                fh.write(f"\t{word}: {alphabetical_words[word]}\n")

            # Words by frequency
            fh.write("Words (by frequency):\n")
            for word, count in top_words.items():
                fh.write(f"\t{word}: {count}\n")

            # Symbols alphabetical
            fh.write("Symbols (alphabetical order):\n")
            for sym in alphabetical_symbols:
                fh.write(f"\t{sym}: {alphabetical_symbols[sym]}\n")

            # Symbols by frequency
            fh.write("Symbols (by frequency):\n")
            for sym, count in top_symbols.items():
                fh.write(f"\t{sym}: {count}\n")

            # Bigrams
            if top_word_pairs:
                fh.write("Word pairs (alphabetical order):\n")
                for pair in alphabetical_word_pairs:
                    fh.write(f"\t{pair}: {alphabetical_word_pairs[pair]}\n")

                fh.write("Word pairs (by frequency):\n")
                for pair, count in top_word_pairs.items():
                    fh.write(f"\t{pair}: {count}\n")

                fh.write("Symbol pairs (alphabetical order):\n")
                for pair in alphabetical_symbol_pairs:
                    fh.write(f"\t{pair}: {alphabetical_symbol_pairs[pair]}\n")

                fh.write("Symbol pairs (by frequency):\n")
                for pair, count in top_symbol_pairs.items():
                    fh.write(f"\t{pair}: {count}\n")

    def write_stats_json(
        self,
        filename,
        source_file,
        stats,
        lower,
        use_stopwords,
        words_without_stopwords_count,
        vocab_size,
        total_symbol_count,
        unique_symbol_count,
        entropy,
        redundancy,
        top_words,
        top_symbols,
        top_word_pairs,
        top_symbol_pairs,
    ):
        """
        Este método escribe en formato JSON las estadísticas de un fichero
            
        :param 
            filename: el nombre del fichero destino.
            source_file: el nombre del fichero fuente.
            stats: las estadíticas del texto.
            use_stopwords: booleano, si se han utilizado stopwords
            words_without_stopwords_count: numero palabras sin las stopwords
            vocab_size: numero de diferentes palabras
            total_symbol_count: numero total de caracteres
            unique_symbol_count: numero de caracteres diferentes
            entropy: valor de entropy calculado
            redundancy: valor de redundancia calculado
            top_words: palabras mas frecuentes
            top_symbols: simbolos mas frecuentes
            top_word_pairs: bigrams mas frecuentes en frases
            top_symbol_pairs: bigrams mas frecuentes en simbolos
    
        :return: None

        Write statistics in JSON format.
        """

        js = {
            "metadata": {
                "source_file": source_file,
                "options": {
                    "lower": lower,
                    "stopwords": use_stopwords,
                    "bigrams": 'biword' in stats,
                    "entropy": 'entropy' in stats
                }
            },
            "basic_stats": {
                "lines": stats['nlines'],
                "words": stats['nwords']
            }
        }

        if use_stopwords:
            js['basic_stats']['words_without_stopwords'] = words_without_stopwords_count

        js['basic_stats'].update({
                "vocab_size": vocab_size,
                "symbols": total_symbol_count,
                "unique_symbols": unique_symbol_count
            })

        if entropy:
            js["entropy_analysis"] = {
                    "shannon_entropy": entropy,
                    "redundancy": redundancy
                }
        
        js["top_words"] = top_words
        js["top_symbols"] = top_symbols

        if top_word_pairs:
            js["top_word_pairs"] = top_word_pairs
            js["top_symbol_pairs"] = top_symbol_pairs

        with open(filename, 'w', encoding='utf-8') as fh:
            json.dump(js, fh, indent=4, ensure_ascii=False)


    def file_stats(
        self,
        filename,
        lower,
        stopwordsfile,
        bigrams,
        full,
        entropy,
        use_json,
    ):
        """
        Este método calcula las estadísticas de un fichero de texto

        :param 
            filename: el nombre del fichero.
            lower: booleano, se debe pasar todo a minúsculas?
            stopwordsfile: nombre del fichero con las stopwords o None si no se aplican
            bigram: booleano, se deben calcular bigramas?
            full: booleano, se deben montrar la estadísticas completas?
            entropy_bool: booleano, se debe calcular la entropía de Shannon?
            use_json: booleano, se debe mostrar las estadísticas en formato JSON?
        :return: None
        """

        stopwords = [] if stopwordsfile is None else open(stopwordsfile, encoding='utf-8').read().split()

        # variables for results

        stats = {
                'nwords': 0,
                'nlines': 0,
                'word': {},
                'symbol': {},
                }

        if bigrams:
            stats['biword'] = {}
            stats['bisymbol'] = {}

        if entropy:
            stats['entropy'] = None
            stats['redundancy'] = None
        
        def add_to_dict(word, dict):
            if word in dict.keys():
                dict[word] += 1
            else:
                dict[word] = 1

        def add_word(word):
            add_to_dict(word, stats['word'])
            
        def add_symbol(word):
            add_to_dict(word, stats['symbol'])
        
        top_word_pairs = {}
        top_symbol_pairs = {}

        with open(filename, 'r') as file:
            for line in file:
                stats['nlines'] += 1

                if lower:
                    line = line.lower()
                
                line = self.clean_re.sub(' ', line)
                words = line.split()

                if not words:
                    continue

                stats['nwords'] += len(words)

                if bigrams:
                    word_bigrams = get_ngrams(words.copy(), 2, True)
                    for bigram in word_bigrams:
                        # Ignore bigrams containing stopwords
                        if bigram[0] in stopwords or bigram[1] in stopwords:
                            continue

                        pair = f"{bigram[0]} {bigram[1]}"
                        add_to_dict(pair, top_word_pairs)

                # Remove stopwords
                words = [word for word in words if word not in stopwords]

                for word in words:
                    add_word(word)

                    if bigrams:
                        symbol_bigrams = get_ngrams(list(word), 2, False)
                        for bigram in symbol_bigrams:
                            pair = f"{bigram[0]}{bigram[1]}"
                            add_to_dict(pair, top_symbol_pairs)
                    
                    for symbol in word:
                        add_symbol(symbol)

        # Generating the new filename
        options_str = '_'
        options_str += 'l' if lower else ''
        options_str += 's' if stopwordsfile else ''
        options_str += 'b' if bigrams else ''
        options_str += 'f' if full else ''
        options_str += 'e' if entropy else ''
        options_str += 'j' if use_json else ''
        options_str = '' if len(options_str) == 1 else options_str  # Clean the options string if it only contains `_`
        new_filename = filename.split('.')[0] + options_str + '_stats' + ('.json' if use_json else '.txt')
        
        # Count unique symbols and words
        unique_symbol_count = len(stats['symbol'].keys())
        vocab_size = len(stats['word'].keys())

        # Count total symbols and words without stopwords
        total_symbol_count = 0
        for k, v in stats['symbol'].items():
            total_symbol_count += v
        
        words_without_stopwords_count = 0
        for k, v in stats['word'].items():
            words_without_stopwords_count += v

        # Sort frequency dictionaries and trim them if necessary
        word_freq_sorted = sort_dic_by_values(stats['word'])
        symbol_freq_sorted = sort_dic_by_values(stats['symbol'])
        top_word_pairs_sorted = sort_dic_by_values(top_word_pairs)
        top_symbol_pairs_sorted = sort_dic_by_values(top_symbol_pairs)

        alphabetical_words = dict(sorted(stats['word'].items()))
        alphabetical_symbols = dict(sorted(stats['symbol'].items()))
        alphabetical_word_pairs = dict(sorted(top_word_pairs.items()))
        alphabetical_symbol_pairs = dict(sorted(top_symbol_pairs.items()))

        if not full:
            word_freq_sorted = word_freq_sorted[:20]
            symbol_freq_sorted = symbol_freq_sorted[:20]
            top_word_pairs_sorted = top_word_pairs_sorted[:20]
            top_symbol_pairs_sorted = top_symbol_pairs_sorted[:20]
            
            alphabetical_words = dict(list(alphabetical_words.items())[:20])
            alphabetical_symbols = dict(list(alphabetical_symbols.items())[:20])
            alphabetical_word_pairs = dict(list(alphabetical_word_pairs.items())[:20])
            alphabetical_symbol_pairs = dict(list(alphabetical_symbol_pairs.items())[:20])
        
        top_words = {word: count for (word, count) in word_freq_sorted}
        top_symbols = {sym: count for (sym, count) in symbol_freq_sorted}
        top_word_pairs = {pair: count for (pair, count) in top_word_pairs_sorted}
        top_symbol_pairs = {pair: count for (pair, count) in top_symbol_pairs_sorted}

        # Calculate entropy and redundancy (if entropy calculations are enabled)
        shannon_entropy = None
        redundancy = None
        if entropy:
            shannon_entropy = self.get_shannon_entropy(stats['symbol'], total_symbol_count)
            redundancy = self.get_redundancy(shannon_entropy, unique_symbol_count)    

        if use_json:
            self.write_stats_json(
                new_filename,
                filename,
                stats,
                lower,
                stopwordsfile is not None,
                words_without_stopwords_count,
                vocab_size,
                total_symbol_count,
                unique_symbol_count,
                shannon_entropy,
                redundancy,
                top_words,
                top_symbols,
                top_word_pairs,
                top_symbol_pairs,
            )
        else:
            self.write_stats_text(
                new_filename,
                stats,
                stopwordsfile is not None,
                words_without_stopwords_count,
                vocab_size,
                total_symbol_count,
                unique_symbol_count,
                shannon_entropy,
                redundancy,
                top_words,
                alphabetical_words,
                top_symbols,
                alphabetical_symbols,
                top_word_pairs,
                alphabetical_word_pairs,
                top_symbol_pairs,
                alphabetical_symbol_pairs
            )

    def compute_files(self, filenames, **args):
        """
        Este mÃ©todo calcula las estadísticas de una lista de ficheros de texto

        :param 
            filenames: lista con los nombre de los ficheros.
            args: argumentos que se pasan a "file_stats".

        :return: None
        """

        for filename in filenames:
            self.file_stats(filename, **args)


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Compute comprehensive statistics from text files.')
    parser.add_argument('file', metavar='file', type=str, nargs='+',
                        help='text files to analyze.')

    parser.add_argument('-l', '--lower', dest='lower',
                        action='store_true', default=False, 
                        help='lowercase all words before computing stats.')

    parser.add_argument('-s', '--stop', dest='stopwords', action='store',
                        help='filename with the stopwords.')

    parser.add_argument('-b', '--bigram', dest='bigram',
                        action='store_true', default=False, 
                        help='compute bigram stats.')

    parser.add_argument('-f', '--full', dest='full',
                        action='store_true', default=False, 
                        help='show full stats (instead of top 20).')
    
    parser.add_argument('-e', '--entropy', dest='entropy',
                        action='store_true', default=False, 
                        help='compute Shannon entropy and redundancy.')
    
    parser.add_argument('-j', '--json', dest='json',
                        action='store_true', default=False,
                        help='output statistics in JSON format.')

    args = parser.parse_args()
    wc = WordCounter()
    wc.compute_files(args.file,
                     lower=args.lower,
                     stopwordsfile=args.stopwords,
                     bigrams=args.bigram,
                     full=args.full,
                     entropy=args.entropy,
                     use_json=args.json)
