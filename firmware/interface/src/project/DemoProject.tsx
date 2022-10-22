
import React, { FC } from 'react';
import { useState, useEffect } from 'react';
import '../style/App.css';
import { UseWindowDimensions } from '../hooks/useWindowDimensions'
import LogoSVG from "../images/LogoBoar.svg"

/**
 * Generate all the lines of the interface
 * @returns A dictionnary containing a group of SVG path and a div
 */
function Lines() {

	/* eslint-disable */

	const [toggle_all, setToggleAll] = useState(true)
	const [animate, setAnimation] = useState(true)

	React.useEffect( () => {
		toggle_all? setTimeout( () => { setAnimation( toggle_all ) },  600 ) : setAnimation( toggle_all )
	}, [ toggle_all ] )

	const [showFullHeader, setFullHeader] = useState(true)
	const [showFullFooter, setFullFooter] = useState(true)

	const { height, width } = UseWindowDimensions()

	const logo_size = 0.42
	const hex_radius : number = Math.min(width * 0.3, height*0.15);
	let icon_size  	: number = hex_radius/10
	const menu_radius: number = hex_radius * 1.2
	const L: number = Math.sqrt( Math.pow( (menu_radius * 2.5), 2 ) - Math.pow( (menu_radius * 1.5), 2) ) + hex_radius/2

	/**
	 * Generate a full column of lines
	 * @param x 		The x position of obstacle with respect to the center of the window
	 * @param R_center 	The radius of the obstacle
	 * @returns 		A dictionnary containing a group of SVG path and a div
	 */
	function column(x: number, R_center:number) {

		let offsets 	= [-2.5, -1.5, -0.5, 0.5, 1.5, 2.5].map(X => X * hex_radius / 6)

		/**
		 * Create a set of 4 lines around an obstacle of a given radius
		 * @param y 		The y position of obstacle with respect to the center of the window
		 * @param R_center 	The radius of the obstacle
		 * @returns 		A dictionnary containing a group of SVG path and a div
		 */
		function setAroundObstacle(y : number, R_center : number, animated : boolean = true) {

			let R_obs	: number = R_center >= 0 ? R_center : (toggle_all ? hex_radius / (animated ? 4.5 : 6 ): 0)
			let sm  	: number = R_obs * 1.5
			let l 		: number = (R_center === 0 && toggle_all) ? 0 : hex_radius

			const [pulse, invPulse] = useState(false);
			const period = Math.random()*1000+1000
	
			/**
			 * Invert the boolean `pulse` every x miliseconds
			 */
			useEffect(() => {
				const id = setTimeout(() => {
					invPulse(animate&&toggle_all ? !pulse : false);
				}, period);
			
				return () => clearTimeout(id);
			  }, [pulse, animate]);

			/**
			 * Create a single line around an obstacle
			 * @param offset 	The offset of the line with respect to the obstacle center
			 * @returns 		A dictionnary containing a group of SVG path and a div
			 */
			function singleAroundObstacle(offset: number) {

				let offset_obs:number = toggle_all ? 0:x

				let R = R_obs + (pulse && animated ? 0.6 : 0.4) * Math.abs(offset + offset_obs)


				let center_border	:boolean = (y===0) && (x!==0)
				let dir				:boolean = ((offset - (center_border ? x:0) )<0) !== center_border
				
				if (Math.abs(offset + offset_obs) > R ) {
					return (
						<path style={(toggle_all && animate) && animated? {transition: `all ${period}ms`} : {}} d={`
							M ${offset}  ${-l/2 + y} 		 
							L ${offset}  ${y}   	
							A ${0} ${0} 		 	0 0 ${dir ? 1 : 0} 		${offset} 	${y} 
							A ${0} ${0} 			0 0 ${dir ? 0 : 1}	 	${offset} 	${y}  
							A ${0} ${0} 		 	0 0 ${dir ? 1 : 0}    	${offset}   ${y}
							L ${offset}  ${l/2 + y}   
							`}/>
					)
				}
			
				let xtilde  : number  = Math.sqrt( Math.pow( (sm + R), 2 ) - Math.pow( (sm + Math.abs(offset + offset_obs)), 2) )
				let xhat    : number  = R * Math.sqrt( Math.pow( (sm + R), 2 ) - Math.pow( (sm + Math.abs(offset + offset_obs)), 2 ) ) / ( R + sm)
				let yhat    : number  = Math.abs(offset + offset_obs) / (offset + offset_obs) * Math.sqrt( Math.pow( R, 2) - Math.pow( xhat, 2)) - offset_obs
			
				return (
					<path style={(toggle_all && animate) && animated? {transition: `all ${period}ms`} : {}} d={`  
							M ${offset}  ${-(xtilde>l ? L:l/2) + y} 		 
							L ${offset}  ${-xtilde + y}   	
							A ${sm} ${sm} 		 	0 0 ${dir ? 1 : 0} 		${yhat} 	${-xhat + y} 
							A ${R}  ${R} 			0 0 ${dir ? 0 : 1}	 	${yhat} 	${xhat + y}  
							A ${sm} ${sm} 		 	0 0 ${dir ? 1 : 0}    	${offset}   ${xtilde + y}
							L ${offset}  ${(xtilde>l ? L:l/2) + y}   		
							`}/>
				)
			}
		
			let set_of_line = []
		
			for (let i = 0; i < offsets.length; i++) {
				set_of_line.push( singleAroundObstacle( offsets[i] ))
			}
		
			return (<g> {set_of_line} </g>)
		}

		/**
		 * Generate a submenu for each button with animated bezier and the button icon
		 * @param y Y position of the obstacle
		 * @returns A dictionnary containing a group of SVG path and a div
		 */
		function SubMenu(y:number) {

			const [toggle, setToggle] = useState(true)

			let sh 			: number = icon_size/2
			
			/**
			 * Generate the i icon and the cross
			 * @returns A group of paths
			 */
			function icon() {
				let icon = 
				 	toggle_all?
						toggle ?
						<g  className='thickSVG' >
							<path d={`  
							M ${0}  ${y-icon_size/2 + sh/2} 		L ${0}  ${y+icon_size/2 + sh/2}   			
							`}/>
							<path d={`  
							M ${0}  ${y-icon_size/2 - sh/2} 		L ${0}  ${y-icon_size/2 - sh/2}   			
							`}/>
						</g> 
						:
						<g  className='thickSVG'  >
							<path d={`  
							M ${-icon_size/2}  	${y-icon_size/2} 	L ${icon_size/2}  	${y+icon_size/2}   			
							`}/>
							<path d={`  
							M ${icon_size/2}  	${y-icon_size/2} 	L ${-icon_size/2}  	${y+icon_size/2}   			
							`}/>
						</g>
					:
						<g  className='thickSVG'>
							<path d={`  
							M ${0}  	${y} 		 				L ${0}  	${y}   			
							`}/>
							<path d={`  
							M ${0}  	${y} 		 				L ${0}  	${y}   			
							`}/>
						</g>
				return <g> {icon} </g>
			} 

			/**
			 * Generate a vertical line from the object to the top or bottom of the window
			 * @param x The x position with respect to the obstacle center
			 * @returns A group of paths
			 */
			function vLine(x:number) {
				return <g> 
						<path d={ 
							`M  ${x} ${y+Math.abs(y)/y*hex_radius/2} 	L  ${x} ${Math.abs(y)/y*height/2}`
						}/>
						</g>
			}

			/**
			 * Generate an animated bezier curve
			 * @param x The x position with respect to the obstacle center
			 * @returns A group of paths
			 */
			function vBezier(x:number) {
					return <g>
						<path d={ toggle? 
									`M  ${x} ${y+Math.abs(y)/y*hex_radius/2} 	Q ${x} ${y+Math.abs(y)/y*height*3/4}    			${x+Math.abs(x)/x*hex_radius/6} ${Math.abs(y)/y*height}`
								:	`M  ${x} ${y+Math.abs(y)/y*hex_radius/2}  	Q ${x} ${y+Math.abs(y)/y*hex_radius*4/6}    	${x+Math.abs(x)/x*hex_radius/6} ${y + Math.abs(y)/y*hex_radius*4/6}`
						}/>
					</g>
			}

			/**
			 * Generate an animated bezier curve
			 * @param x The x position with respect to the obstacle center
			 * @returns A group of paths
			 */
			function vBezierCenter(x:number) {
					return <g>
						<path d={ toggle? 
									`M  ${x} ${y+Math.abs(y)/y*hex_radius/2}  	
									Q ${x} ${Math.abs(y)/y*height*1/4}    ${x} ${Math.abs(y)/y*height*2/4}
									Q ${x} ${Math.abs(y)/y*height*3/4}    ${x} ${Math.abs(y)/y*height*3/4}
									L ${x} ${Math.abs(y)/y*height}`
									:
									`M  ${x} ${y+Math.abs(y)/y*hex_radius/2}  	
									Q ${x} ${y+Math.abs(y)/y*hex_radius*4/6}    ${x-Math.abs(x)/x*hex_radius/6} ${y + Math.abs(y)/y*hex_radius*4/6}
									Q ${x-2*Math.abs(x)/x*hex_radius/6} ${y+Math.abs(y)/y*hex_radius*4/6}    ${x-2*Math.abs(x)/x*hex_radius/6} ${y + Math.abs(y)/y*hex_radius*5/6}
									L ${x-2*Math.abs(x)/x*hex_radius/6} ${Math.abs(y)/y*height/2}`
						}/>
					</g>
			}

			/**
			 * Generate the animated label of the radio
			 * @returns A div with text
			 */
			function radioLabel() { //TO CHANGE
				return  <div
							style={{
								transform:`translate( 
											calc(${width/2 + x}px - 50%),
											calc(${toggle? Math.abs(y)/y*height : -height/2 + y + Math.abs(y)/y*hex_radius*0.8}px + ${y>0? 0:-100}%))`,
							}}
							className={`verticalText animatedDiv bg1`}
							id="rolltext"
							onClick={ () => {setToggleAll(false) }}>
							RADIO
						</div>
			}


			let obstacle = setAroundObstacle(
				R_center === 0 ? Math.abs(y)/y*hex_radius/2 : Math.abs(y)/y*(toggle_all? hex_radius : L),
				-1, toggle)

			return {svg: <g onClick={ () => {if(toggle_all) {setToggle(!toggle) ; if (x===0) { y<0? setFullHeader(!toggle) : setFullFooter(!toggle) } } } } >
						{obstacle}
						{icon()}
						{vBezier(-1.5*hex_radius/6)}
						{vBezier(1.5*hex_radius/6)}
						{vBezierCenter(-0.5*hex_radius/6)}
						{vBezierCenter(0.5*hex_radius/6)}
						{vLine(-2.5*hex_radius/6)}
						{vLine(2.5*hex_radius/6)}
						{radioLabel()}
						<circle className="clickable" cx={0} cy={y} r={2*icon_size} stroke="none" fill="black" fill-opacity="0" />
					</g>, div: radioLabel()}
		}

		let obstacle 	= [  setAroundObstacle( 0, R_center, false) ]

		let y_pos 		= R_center === 0 ?
							[-hex_radius/2 , 0, hex_radius/2]
						: 	[toggle_all? -hex_radius : -L, 0, toggle_all? hex_radius : L ]


		let sub_menus = [SubMenu(y_pos[0]), SubMenu(y_pos[2])]
		obstacle.push(sub_menus[0].svg); obstacle.push(sub_menus[1].svg);

		let div 	= []
		div.push(sub_menus[0].div); div.push(sub_menus[1].div);

		return {svg: (<g transform={`translate(${width/2 + x} ${height/2})`}> {obstacle} </g>), div: <div>{div}</div>}
	}

	let columns 	= []
	let x_pos 		= [-1, 0, 1].map(X => X*hex_radius )
	let Radii_cent  = toggle_all ? [0, hex_radius / 3.5, 0] : [1, 1, 1].map(X => X*menu_radius)

	for (let i = 0; i < x_pos.length; i++) {
		columns.push( column( x_pos[i], Radii_cent[i] ) )
	}

	let columns_svg = columns.map(x => x.svg)
	let columns_div = columns.map(x => x.div)

	/**
	 * Generate one letter of the footer
	 * @param letter The letter
	 * @returns A div containing the letter
	 */
	function Letter(letter:string, x_offest:number, y_offset:string, header:boolean) {
		return <div
				style={{
					transform:`translate(
								calc(${width/2 + x_offest*hex_radius/6}px - 50%),
								calc(${y_offset}))`,
					opacity: `${x_offest<2  && x_offest>-2 && toggle_all? header ? (showFullHeader? 1:0) : (showFullFooter? 1:0) : 1}`
				}}
				className={`verticalText animatedDiv`}>
				{letter}
			</div>
	}

	/**
	 * Generate the footer "By toutatix"
	 * @return A div element
	 */
	 function Header() {
		let all_letters = 'RAYONX V1'
		let footer = all_letters.split('').map(
			(letter, index) => 
			Letter(letter, (index-Math.floor(all_letters.length/2)), `${-height+hex_radius / (toggle_all? 3:10)}px - 0%`, true))
		return  <div>{footer}</div>
	}

	/**
	 * Generate the footer "By toutatix"
	 * @return A div element
	 */
	function Footer() {
		let all_letters = 'BY TOUTATIX'
		let footer = all_letters.split('').map(
			(letter, index) =>
			Letter(letter, (index-Math.floor(all_letters.length/2)), `${-hex_radius / (toggle_all? 3:10)}px - 150%`, false))
		return  <div>{footer}</div>
	}

	/**
	 * Generate the selection menu with the input, the list of choices and the animated valildation button
	 * @returns A div element
	 */
	function SelectionMenu () {

		/** 
		 * Generate the boar logo using the SVG file. Handling animation and responsiveness 
		 * @returns A div element
		 */
		function BoarLogo() {
			return 	<div className="boarLogo animatedDiv" style={{
						top: `calc(50vh - ${toggle_all? hex_radius*logo_size*0.56 : hex_radius*1.4}px)`,
						left: `calc(50vw - ${hex_radius*logo_size/2}px)`
						}}>
						<img 	src={LogoSVG} alt="LogoTOT" id="logoTot" 
								style={{height:`${hex_radius*logo_size}px`, width:`${hex_radius*logo_size}px`}}/>
					</div>
		}

		/** 
		 * Generate the validation button using the SVG.
		 * @returns A svg element
		 */
				 function ValidationButton() { 

					return 	<g>
							{toggle_all ?
							<g  className='thickSVG' style={{opacity:`0`}}>
								<path d={`  
								M ${width/2}  	${height/2} 	L ${width/2}  	${height/2}   			
								`}/>
								<path d={`  
								M ${width/2}  	${height/2} 	L ${width/2}  	${height/2}   			
								`}/>
							</g>
							:
							<g  className='thickSVG'>
								<path d={`  
								M ${width/2-icon_size/2}  	${height/2-icon_size/2+hex_radius} 	L ${width/2+icon_size/2}  	${height/2+icon_size/2+hex_radius}   			
								`}/>
								<path d={`  
								M ${width/2+icon_size/2}  	${height/2-icon_size/2+hex_radius} 	L ${width/2-icon_size/2}  	${height/2+icon_size/2+hex_radius}   			
								`}/>
							</g>}
							<circle className={`${toggle_all? {} : "clickable"}`}
									onClick={() => {if (!toggle_all) {setToggleAll(true)}}}
									cx={width/2} cy={height/2 + hex_radius} r={2*icon_size}
									stroke="none" fill="black" fill-opacity="0" />
							</g>
				}

		return 	{div: <div>
					{BoarLogo()}
					<div className="selectionMenu animatedDiv" 
						style={{opacity: `${toggle_all? 0:1}`}}>
						<div> Select</div>
						<input placeholder="Radio's name"></input>
						<div>
							Radio 1
						</div>
					</div>
					{ValidationButton()}
				</div>,
				svg: ValidationButton()}
	}

	let selection_menu = SelectionMenu()

	return {svg:<svg> {selection_menu.svg}{columns_svg}</svg>,
			div:<div>
					{columns_div}
					{Footer()}
					{Header()}
					{selection_menu.div}
				</div>}

}


const DemoProject: FC = () => {
  let lines = Lines()

	return (
		<main className="App bg">
				{lines.svg}
				{lines.div}
		</main>
	);
};

export default DemoProject;
