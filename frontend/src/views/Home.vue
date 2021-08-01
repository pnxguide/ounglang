<template>
  <div class="wrapper">
    <div class="wrapper-cover"></div>
    <div class="header-wrapper">
      <img
        src="@/assets/images/logo.png"
        alt="ounglang, oung oung oung, what is oung"
        class="oung-logo"
      />
      <button class="open-editor-button" @click="goToEditor">
        <span
          class="mdi mdi-play-circle"
          style="font-size: 30px; margin-right: 10px"
        ></span
        >&nbsp;Open Editor
      </button>
    </div>
    <div class="about-wrapper">
      <div class="about-content">
        <p style="margin: 0">
          Lorem ipsum dolor sit amet consectetur adipisicing elit. Rem, optio
          ullam! Eum praesentium, velit dolore labore, sed perspiciatis cumque
          dicta harum ipsa architecto inventore! At corporis qui commodi laborum
          aspernatur. Aliquid atque nam voluptatum molestias animi sint esse cum
          placeat impedit voluptates ut, reprehenderit tempore ad nobis,
          perspiciatis sed expedita. Incidunt quidem, quis quia corporis qui
          sunt nemo eum vitae! Recusandae doloribus natus explicabo alias in,
          debitis similique? Corrupti, voluptates enim perspiciatis cumque
          beatae perferendis quo! Eveniet minima exercitationem expedita
          doloribus doloremque, in aliquid perspiciatis, et laboriosam
          accusamus, illo consequuntur? Natus eaque temporibus, laboriosam fuga
          exercitationem quas, assumenda repudiandae id, asperiores delectus
          velit eveniet ipsum corrupti omnis ratione illum. Rerum obcaecati
          tenetur quae dignissimos magni saepe impedit perferendis nam ducimus.
          Perspiciatis amet laudantium quis, similique suscipit esse
          reprehenderit doloremque facere. Aperiam nihil distinctio quia!
          Ratione distinctio repellat molestiae qui. Voluptates repellat nihil
          nam nostrum beatae libero quibusdam explicabo cumque iste!
        </p>
      </div>
    </div>
  </div>
</template>

<script>
import router from '@/router';
import audio from '@/assets/audios/home.mp3';
import { onBeforeUnmount, onMounted } from '@vue/runtime-core';

export default {
  setup(props, context) {
    const goToEditor = () => {
      router.push('/editor');
    };
    const player = new Audio(audio);
    player.volume = 0.1;
    player.playbackRate = 2;
    player.loop = true;
    player.currentTime = 11;

    const playAudio = (e) => {
      if (player.paused) player.play();
    };

    player.ontimeupdate = (e) => {
      if (player.currentTime >= 16) {
        player.currentTime = 11;
      }
    };

    onMounted(() => {
      document.addEventListener('mousemove', playAudio);
    });

    onBeforeUnmount(() => {
      player.pause();
      document.removeEventListener('mousemove', playAudio);
    });

    return { goToEditor };
  },
};
</script>

<style lang="scss" scoped>
@import url('https://fonts.googleapis.com/css2?family=Poppins:wght@400;500;700&display=swap');

.wrapper {
  font-family: 'Poppins', sans-serif;

  position: relative;
  display: flex;
  flex-direction: column;
  align-items: center;
  width: 100%;
  min-height: 100vh;
  background-image: linear-gradient(to bottom right, #a8a8a8, #8a8a8a);
  background-size: 500px 500px;
  background-attachment: fixed;
  animation: animateBackground 5s linear infinite;
  .wrapper-cover {
    position: absolute;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    background-color: rgba(0, 0, 0, 0.7);
    & ~ * {
      position: relative;
    }
  }
}

@keyframes animateBackground {
  to {
    background-position: 500px 500px;
  }
}

.header-wrapper {
  display: flex;
  flex-direction: column;
  align-items: center;
  min-height: calc(80vh);
  .open-editor-button {
    font-family: 'Poppins', sans-serif;

    display: flex;
    align-items: center;
    margin-top: auto;
    margin-bottom: 50px;
    padding: 10px 40px;
    max-width: 300px;
    color: white;
    font-weight: bold;
    font-size: 20px;
    outline: none;
    border: none;
    border-radius: 10px;
    box-shadow: 5px 5px 0 black;
    background-color: #0db030;
    cursor: pointer;
    &:active {
      transform: translate(5px, 5px);
      box-shadow: none;
    }
  }
  .oung-logo {
    margin: auto 0;
    max-width: 500px;
    width: 100%;
    height: auto;
    user-select: none;
    pointer-events: none;
    animation: superOungDekWance 0.5s linear infinite;
  }
}

@keyframes superOungDekWance {
  0%,
  100% {
    transform: scale(1);
  }
  50% {
    transform: scale(0.9);
  }
}

.about-wrapper {
  margin: auto;
  padding: 30px;
  width: 100%;
  max-width: 1200px;
  min-height: 100px;
  border-top-left-radius: 20px;
  border-top-right-radius: 20px;
  background-color: #d8d8d8;
  color: black;

  .about-content * {
    line-height: 1.5;
    font-weight: 400;
  }
}
</style>
