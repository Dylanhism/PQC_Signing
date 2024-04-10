import configparser

config = configparser.ConfigParser()
config.read('config.ini')

seed_len_128f = int(config['Signing']['seed_len_128f'])
pem_key_folder = config['Paths']['pem_key_folder']

menu:str = """
SPHINCS SHAKE_128f SIGNATURE GENERATOR
 * By Liam Kelly and Dylan Hughes
 * March 2024
 * License: MIT
"""

