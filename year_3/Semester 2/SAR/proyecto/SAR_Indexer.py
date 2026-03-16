import argparse
import pickle
import sys
import time

from SAR_lib import SAR_Indexer


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Index a directory with Wikipedia articles in json format.')
    parser.add_argument('dir', type=str,
                        help='directory with the Wikipedia articles.')

    parser.add_argument('index', metavar='index', type=str,
                        help='name of the index.')

    parser.add_argument('-P', '--positional', dest='positional', action='store_true', default=False, 
                    help='compute positional index.')

    parser.add_argument('-S', '--semantic', dest='semantic', action='store_true', default=False, 
                    help='compute the semantic index.')


    args = parser.parse_args()

    indexer = SAR_Indexer()
    t0 = time.time()
    indexer.index_dir(args.dir, **vars(args))
    t1 = time.time()
    indexer.save_info(args.index)
    t2 = time.time()
    indexer.show_stats()
    print("Time indexing: %2.2fs." % (t1 - t0))
    print("Time saving: %2.2fs." % (t2 - t1))
    print()

