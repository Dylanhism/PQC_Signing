import config
import os
import pyspx.shake_128f
import pyspx.shake_192f
import pyspx.shake_256f
import pyspx.haraka_128f
import pyspx.haraka_192f
import pyspx.haraka_256f
import secrets


def prepare_signature(message: bytes, type: str):
    seed = ' '

    public_key, secret_key, signature = ' ', ' ', ' '
    if type=='shake_128f':
        seed = secrets.token_bytes(config.seed_len_128f)
        public_key, secret_key = pyspx.shake_128f.generate_keypair(seed)
        signature = pyspx.shake_128f.sign(message, secret_key)
    elif type == 'shake_192f':
        seed = secrets.token_bytes(config.seed_len_192f)
        public_key, secret_key = pyspx.shake_192f.generate_keypair(seed)
        signature = pyspx.shake_192f.sign(message, secret_key)
    elif type == 'shake_256f':
        seed = secrets.token_bytes(config.seed_len_256f)
        public_key, secret_key = pyspx.shake_256f.generate_keypair(seed)
        signature = pyspx.shake_256f.sign(message, secret_key)
    elif type == 'haraka_128f':
        seed = secrets.token_bytes(config.seed_len_128f)
        public_key, secret_key = pyspx.haraka_192f.generate_keypair(seed)
        signature = pyspx.haraka_192f.sign(message, secret_key)
    elif type == 'haraka_192f':
        seed = secrets.token_bytes(config.seed_len_192f)
        public_key, secret_key = pyspx.haraka_192f.generate_keypair(seed)
        signature = pyspx.haraka_192f.sign(message, secret_key)
    elif type == 'haraka_256f':
        seed = secrets.token_bytes(config.seed_len_256f)
        public_key, secret_key = pyspx.haraka_256f.generate_keypair(seed)
        signature = pyspx.haraka_256f.sign(message, secret_key)

    return public_key, signature

def batch_process(path_to_files, type='shake_128f'):
    if not os.path.exists(path_to_files):
        print(f"Error: Directory '{path_to_files}' does not exist.")
        return

    for root, _, files in os.walk(path_to_files):
        for file_name in files:
            if not (file_name.startswith('.') or file_name.endswith('.pem') or file_name.endswith('.pub')):  # Reject hidden files and other PEMs
                file_path = os.path.join(root, file_name)
                try:
                    with open(file_path, 'rb') as file:
                        file_bytes = file.read()
                        pk, sign = prepare_signature(file_bytes, type)

                        pem_path = file_path + '.pem'
                        with open(pem_path, 'wb') as pem:
                            pem.write(sign)
                            print(f"PEM generated for '{file_path}'.")

                        pub_path = file_path + '.pub'
                        with open(pub_path, 'wb') as pem:
                            pem.write(pk)
                            print(f"PUB generated for '{file_path}'.")

                except PermissionError:
                    print(f"Permission denied for file '{file_path}'.")
                except IOError as e:
                    print(f"An I/O error occurred for file '{file_path}': {e}")
                except Exception as e:
                    print(f"An unexpected error occurred for file '{file_path}': {e}")

def batch_verify(path_to_files, type='shake_128f'):
    if not os.path.exists(path_to_files):
        print(f"Error: Directory '{path_to_files}' does not exist.")
        return

    for root, _, files in os.walk(path_to_files):
        for file_name in files:
            if file_name.endswith('.pub'):
                file_path_pub = os.path.join(root, file_name)
                try:
                    with open(file_path_pub, 'rb') as pub_file:
                        pub_bytes = pub_file.read()
                        pem_path = os.path.join(root, file_name[:-4]+".pem")
                        with open(pem_path, 'rb') as pem_file:
                            pem_bytes = pem_file.read()
                            file_path = os.path.join(root, file_name[:-4])
                            with open(file_path, 'rb') as file:
                                file_bytes = file.read()

                                if type == 'shake_128f':
                                    print(f"Verification using '{file_path}' is: {pyspx.shake_128f.verify(file_bytes, pem_bytes, pub_bytes)}")
                                elif type == 'shake_192f':
                                    print(f"Verification using '{file_path}' is: {pyspx.shake_192f.verify(file_bytes, pem_bytes, pub_bytes)}")
                                elif type == 'shake_256f':
                                    print(f"Verification using '{file_path}' is: {pyspx.shake_256f.verify(file_bytes, pem_bytes, pub_bytes)}")
                                elif type == 'haraka_128f':
                                    print(f"Verification using '{file_path}' is: {pyspx.haraka_128f.verify(file_bytes, pem_bytes, pub_bytes)}")
                                elif type == 'haraka_192f':
                                    print(f"Verification using '{file_path}' is: {pyspx.haraka_192f.verify(file_bytes, pem_bytes, pub_bytes)}")
                                elif type == 'haraka_256f':
                                    print(f"Verification using '{file_path}' is: {pyspx.haraka_256f.verify(file_bytes, pem_bytes, pub_bytes)}")

                except PermissionError:
                    print(f"Permission denied for file '{file_path}'.")
                except IOError as e:
                    print(f"An I/O error occurred for file '{file_path}': {e}")
                except Exception as e:
                    print(f"An unexpected error occurred for file '{file_path}': {e}")

if __name__ == '__main__':
    print(config.menu)
    while True:
        folder_path = input('>> Input full path of folder containing files to generate for (or type -1 to exit): ')
        if folder_path == '-1':
            print('Thank you, come again!')
            break
        batch_process(folder_path)
