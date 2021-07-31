<template>
  <div class="editor-wrapper">
    <div class="editor" ref="editorRef"></div>
    <div class="helper">
      <h3 class="title">OUNG Helper</h3>
      <hr class="divider" />
      <div class="buttons">
        <button
          class="button"
          v-for="key in Object.keys(oungHelpers)"
          @click="appendToEditor(oungHelpers[key])"
        >
          {{ key }}
        </button>
      </div>
    </div>
  </div>
</template>

<script>
import * as monaco from 'monaco-editor';
import { ref } from '@vue/reactivity';
import { onMounted } from '@vue/runtime-core';
import oungHelpers from '@/oung_editor/helpers';

export default {
  setup() {
    const editorRef = ref(null);
    let editor = null;

    onMounted(() => {
      editor = monaco.editor.create(editorRef.value, {
        language: 'oung',
        glyphMargin: 0,
        theme: 'oung',
        wordWrap: true,
      });

      editor.onDidChangeCursorPosition((e) => {
        const prefix = getCharactorBeforeCursor(
          e.position.lineNumber,
          e.position.column,
        );
        if (prefix == 'oung') {
          console.log('this is oung sound');
        }
      });
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

    return { editorRef, oungHelpers, appendToEditor };
  },
};
</script>

<style lang="scss" scoped>
$helperWidth: 350px;

.editor-wrapper {
  display: flex;
  flex-direction: row;
  width: 100vw;
  height: 100vh;
  background-color: #1e1e1e;
  overflow: hidden;
}
.editor {
  flex-grow: 1;
  height: 100%;
}
.helper {
  display: flex;
  flex-direction: column;
  padding: 16px;
  min-width: $helperWidth !important;
  overflow: hidden auto;
  .title {
    margin: 0;
    color: white;
    font-weight: bold;
    letter-spacing: 5px;
  }
  .buttons {
    margin-top: 5px;
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    grid-gap: 16px;
    .button {
      outline: none;
      border: none;
      height: 40px;
      font-size: 18px;
      font-weight: bold;
      user-select: none;
      background-color: #353535;
      color: white;
      border-radius: 5px;
      cursor: pointer;
      transition: background-color 0.3s ease;
      &:active {
        transform: scale(0.9);
      }
      &:hover {
        background-color: #505050;
      }
    }
  }
}
</style>
