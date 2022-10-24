import { useState, useEffect } from 'react';

export function UseWindowDimensions() {

  const hasWindow = typeof window !== 'undefined';

  function getWindowDimensions() {
    const width  : number = hasWindow ? window.innerWidth : 720;
    const height : number = hasWindow ? window.innerHeight : 720;
    return {
      width,
      height,
    };
  }

  const [windowDimensions, setWindowDimensions] = useState(getWindowDimensions());

  function HandleResize() {
    setWindowDimensions(getWindowDimensions());
  }

  useEffect(() => {
    if (hasWindow) {
      window.addEventListener('resize', HandleResize);
      return () => window.removeEventListener('resize', HandleResize);
    }
  }, [hasWindow]);

  return windowDimensions;
}