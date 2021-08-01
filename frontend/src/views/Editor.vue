<template>
  <div class="editor-wrapper">
    <div class="editor" ref="editorRef"></div>
    <div class="terminal-background">
      <div class="terminal-wrapper">
        <div class="terminal" ref="terminalRef"></div>
      </div>
    </div>
  </div>
  <div class="open-keyboard" @click="toggleKeyboard()">
    <span class="mdi mdi-keyboard"></span>
  </div>
  <keyboard
    :opened="keyboardOpened"
    :onClose="toggleKeyboard"
    :onPress="onKeyboardPress"
  />
</template>

<script>
import * as monaco from 'monaco-editor';
import { ref } from '@vue/reactivity';
import { inject, onBeforeUnmount, onMounted } from '@vue/runtime-core';
import keyboard from '@/components/Keyboard.vue';
import { Terminal } from 'xterm';
import { FitAddon } from 'xterm-addon-fit';
import oungSound from '@/assets/audios/editor.mp3';

import 'xterm/css/xterm.css';

export default {
  components: { keyboard },
  setup() {
    const editorRef = ref(null);
    const terminalRef = ref(null);
    const keyboardOpened = ref(false);
    const axios = inject('axios');
    const oungAudio = new Audio(oungSound);
    oungAudio.volume = 0.3;

    let editor = null;

    const termPrompt = '/ounglang $ ';
    let cmd = '';
    const term = new Terminal({
      lineHeight: 1.5,
    });
    const fitAddon = new FitAddon();
    term.loadAddon(fitAddon);

    onMounted(() => {
      term.open(terminalRef.value);
      fitAddon.fit();

      term.prompt = function () {
        term.writeln('\r\n' + termPrompt);
      };

      term.setOption('cursorBlink', true);
      term.writeln('────────────────────────────────────────────');
      term.writeln('                                            ');
      term.writeln('         🔥  Welcome to ounglang 🔥         ');
      term.writeln('    👉  Type "run" to run the program 👈   ');
      term.writeln('  👉  Type "clear" to clear the console 👈 ');
      term.writeln('                                            ');
      term.writeln('────────────────────────────────────────────');
      term.prompt();

      term.onKey((e) => {
        if (e.domEvent.code == 'Enter') {
          runCommand();
        } else if (e.domEvent.code == 'Backspace') {
          cmd = cmd.substring(0, cmd.length - 1);
          term.write('\b \b');
        }
      });

      term.onData((data) => {
        cmd += data;
        term.write(data);
      });

      const runCommand = async () => {
        term.write('\r\n');
        if (/clear/.test(cmd)) {
          term.clear();
        } else if (/run.*/.test(cmd)) {
          term.writeln('running oung program ...');
          const params = cmd.split(' ');
          try {
            const { data } = await axios.post(
              'https://ounglang-interpreter.herokuapp.com/run',
              {
                program: editor.getValue().replaceAll(/\n|\r/g, '\n'),
                input: params.splice(1).join(' '),
              },
            );
            term.writeln(data.toString());
          } catch (e) {
            term.writeln('⚠ something went wrong. ⚠');
          }
        } else {
          term.writeln('command not found');
        }
        cmd = '';
        term.prompt();
      };

      editor = monaco.editor.create(editorRef.value, {
        language: 'oung',
        glyphMargin: 0,
        theme: 'oung',
        wordWrap: true,
        fontSize: 20,
      });

      editor.onDidChangeCursorPosition((e) => {
        const prefix = getCharactorBeforeCursor(
          e.position.lineNumber,
          e.position.column,
        );
        if (prefix == 'oung') {
          if (!oungAudio.paused) {
            oungAudio.pause();
            oungAudio.currentTime = 0;
          }
          oungAudio.play();
        }
      });
    });

    onBeforeUnmount(() => {
      editor.dispose();
    });

    const getCharactorBeforeCursor = (line, column) => {
      const text = editor.getValue().split('\n')[line - 1].toLowerCase();
      if (column < 5) {
        return '';
      }
      return text.substr(column - 5, 4);
    };

    const numberToBinary = (number) => {
      return `00000000${number.toString(2)}`.substr(-8);
    };

    const binaryToOung = (binary) => {
      return binary
        .toString()
        .split('')
        .map((e) => {
          if (e == 0) return 'oung';
          return 'OUNG';
        })
        .join(' ');
    };

    const appendToEditor = (number) => {
      editor.trigger('keyboard', 'type', {
        text: binaryToOung(numberToBinary(number)),
      });
      editor.focus();
    };

    const toggleKeyboard = () => {
      keyboardOpened.value = !keyboardOpened.value;
    };

    const onKeyboardPress = (key) => {
      appendToEditor(key);
    };

    return {
      editorRef,
      terminalRef,
      keyboardOpened,
      toggleKeyboard,
      onKeyboardPress,
    };
  },
};
</script>

<style lang="scss" scoped>
$helperWidth: 350px;

.editor-wrapper {
  display: flex;
  flex-direction: column;
  width: 100vw;
  height: 100vh;
  background-color: #1e1e1e;
  overflow: hidden;
}
.editor {
  flex-grow: 1;
  width: 100%;
  height: 100%;
}
.open-keyboard {
  position: fixed;
  display: flex;
  justify-content: center;
  align-items: center;
  bottom: 370px;
  right: 20px;
  width: 50px;
  height: 50px;
  border-radius: 100%;
  font-size: 30px;
  background-color: #e5e5e5;
  cursor: pointer;
}
.terminal-background {
  width: 100%;
  min-height: 350px;
  max-height: 350px;
  background-color: black;
}
.terminal-wrapper {
  position: relative;
  padding: 10px 10px 10px 10px;
  width: 100%;
  height: 100%;
}
.terminal {
  width: 100%;
  height: 100%;
}
</style>
