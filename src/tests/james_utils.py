import gdb
import os
import asyncio
import qemu.qmp

class JamesUtils():
    def TestMemoryRegion(self, expected, start_memory_address):
        memory_array = gdb.inferiors()[0].read_memory(\
                start_memory_address, len(expected)).tobytes()
        for i in range(len(expected)):
            if expected[i] != memory_array[i]:
                return False
        return True

    def TestDiskLoadOnMemory(self, function, file, offset):
        address = gdb.parse_and_eval(f'{function}')
        length = os.path.getsize(f'{file}') - int(offset)

        os_file = open(file, 'rb')
        os_bytes = os_file.read()[0x200:]

        return self.TestMemoryRegion(os_bytes, address)

    def GdbInit(self, symbol_file = '../kcore/kernel_main.bin.elf'):
        gdb.execute('set arch i386')
        gdb.execute(f'file {symbol_file}')
        gdb.execute('target remote localhost:1234')
        gdb.execute('break entry')
        gdb.execute('set logging file gdb_log.txt')
        gdb.execute('set logging overwrite on')
        gdb.execute('set logging enabled on')
        gdb.execute('set confirm off')
        gdb.execute('set pagination off')
        gdb.execute('continue')

    def GdbStop(self):
        gdb.execute('quit')

    async def QmpCommand(self, command, args = {}):
        while True:
            try:
                qmp = qemu.qmp.QMPClient('jamesOS')
                await qmp.connect(('127.0.0.1', 4444))
                res = await qmp.execute(command, args)
                await qmp.disconnect()
                return res
            except qemu.qmp.protocol.ConnectError:
                continue
            except Exception as e:
                print(e)
                break
