"""General key class."""

# SPDX-License-Identifier: Apache-2.0

import sys

AUTOGEN_MESSAGE = "/* Autogenerated by imgtool.py, do not edit. */"

class KeyClass(object):
    def _emit(self, header, trailer, encoded_bytes, indent, file=sys.stdout, len_format=None):
        print(AUTOGEN_MESSAGE, file=file)
        print(header, end='', file=file)
        for count, b in enumerate(encoded_bytes):
            if count % 8 == 0:
                print("\n" + indent, end='', file=file)
            else:
                print(" ", end='', file=file)
            print("0x{:02x},".format(b), end='', file=file)
        print("\n" + trailer, file=file)
        if len_format is not None:
            print(len_format.format(len(encoded_bytes)), file=file)

    def emit_c_public(self, file=sys.stdout):
        self._emit(
                header="const unsigned char {}_pub_key[] = {{".format(self.shortname()),
                trailer="};",
                encoded_bytes=self.get_public_bytes(),
                indent="    ",
                len_format="const unsigned int {}_pub_key_len = {{}};".format(self.shortname()),
                file=file)

    def emit_rust_public(self, file=sys.stdout):
        self._emit(
                header="static {}_PUB_KEY: &[u8] = &[".format(self.shortname().upper()),
                trailer="];",
                encoded_bytes=self.get_public_bytes(),
                indent="    ",
                file=file)

    def emit_private(self, minimal, file=sys.stdout):
        self._emit(
                header="const unsigned char enc_priv_key[] = {",
                trailer="};",
                encoded_bytes=self.get_private_bytes(minimal),
                indent="    ",
                len_format="const unsigned int enc_priv_key_len = {};",
                file=file)
