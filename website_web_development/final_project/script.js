window.onload = () => {
  const transition_el = document.querySelector('.transition');
  const transitions = document.querySelectorAll('a.menu-transition');

  setTimeout(() => {
    transition_el.classList.remove('is-active');
  }, 500);

  for(let i = 0; i < transitions.length; i++){
    const transition = transitions[i];

    transition.addEventListener('click', event_ => {
      event_.preventDefault();

      let target = event_.target.href;

      transition_el.classList.add('is-active');

      setTimeout(() => {
        window.location.href = target;
      }, 500);
    });
  }
}
