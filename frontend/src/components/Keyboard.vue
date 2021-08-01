<template>
  <div class="keyboard" :class="{ opened: opened }">
    <div class="controller">
      <div class="control-button" @click="onClose()">
        <span class="mdi mdi-chevron-down"></span>
      </div>
    </div>
    <div class="keyboard-buttons">
      <div class="row" v-for="row in keyboardData.keys">
        <div
          class="keyboard-button"
          v-for="key in row"
          :style="{
            width: key == 'space' ? '300px' : '50px',
            pointerEvents: key == '' ? 'none' : 'unset',
            backgroundColor: key == '' ? 'transparent' : '#353535',
          }"
          @click="mapKeyPress(key)"
        >
          <span v-if="key.length == 1">
            {{
              isShift
                ? keyboardData.extraShift[key]
                  ? keyboardData.extraShift[key]
                  : key.toUpperCase()
                : key.toLowerCase()
            }}</span
          >
          <span v-else>{{ key.toUpperCase() }}</span>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { onBeforeUnmount, onMounted, ref, toRefs } from 'vue';
import keyboardData from '@/oung_editor/keyboard';
export default {
  props: {
    keyboardOpened: {
      type: Boolean,
      default: false,
    },
    onClose: {
      type: Function,
      default: () => {},
    },
    onPress: {
      type: Function,
      default: () => {},
    },
  },

  setup(props) {
    const { opened, onClose, onPress } = toRefs(props);
    const isShift = ref(false);

    const mapKeyPress = (key) => {
      if (key.length == 1) {
        if (isShift.value) {
          if (keyboardData.extraShift[key]) key = keyboardData.extraShift[key];
          onPress.value(key.toUpperCase().charCodeAt(0));
          return;
        }

        if (/[0-9]/.test(key)) {
          onPress.value(parseInt(key));
          return;
        }

        onPress.value(key.charCodeAt(0));
        return;
      } else {
        onPress.value(keyboardData.customCodes[key.toLowerCase()]);
      }
    };

    const shiftDown = (e) => {
      if (e.code == 'ShiftLeft' || e.code == 'ShiftRight') {
        isShift.value = true;
      }
    };

    const shiftUp = (e) => {
      if (e.code == 'ShiftLeft' || e.code == 'ShiftRight') {
        isShift.value = false;
      }
    };

    onMounted(() => {
      window.addEventListener('keydown', shiftDown);
      window.addEventListener('keyup', shiftUp);
    });

    onBeforeUnmount(() => {
      window.removeEventListener('keydown', shiftDown);
      window.removeEventListener('keyup', shiftUp);
    });

    return { opened, onClose, onPress, keyboardData, mapKeyPress, isShift };
  },
};
</script>

<style lang="scss" scoped>
$keyboard-height: 350px;

.keyboard {
  position: fixed;
  display: flex;
  flex-direction: column;
  padding: 10px;
  width: 100%;
  height: $keyboard-height;
  left: 0;
  bottom: -$keyboard-height;
  background-color: #404040;
  z-index: 10;
  transition: bottom 0.3s ease;
  &[opened='true'] {
    bottom: 0;
  }
}

.keyboard-buttons {
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  gap: 10px;
  width: 100%;
  .row {
    display: flex;
    gap: 10px;
  }
  .keyboard-button {
    display: flex;
    justify-content: center;
    align-items: center;
    min-width: 50px;
    min-height: 50px;
    font-size: 20px;
    color: white;
    font-weight: bold;
    background-color: #353535;
    cursor: pointer;
    user-select: none;
    transition: background-color 0.3s ease;
    &:active {
      transform: scale(0.8);
    }
    &:hover {
      background-color: #505050 !important;
    }
  }
}

.controller {
  display: flex;
  justify-content: flex-end;
  width: 100%;
  .control-button {
    display: flex;
    justify-content: center;
    align-items: center;
    width: 30px;
    height: 30px;
    font-size: 20px;
    color: white;
    background-color: #505050;
    cursor: pointer;
    border-radius: 5px;
  }
}
</style>
