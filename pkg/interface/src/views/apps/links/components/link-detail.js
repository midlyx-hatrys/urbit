import React, { Component } from 'react';
import { TabBar } from '~/views/components/chat-link-tabbar';
import { LinkPreview } from './lib/link-preview';
import { LinkSubmit } from './lib/link-submit';
import { SidebarSwitcher } from '~/views/components/SidebarSwitch';
import { Link } from 'react-router-dom';
import { Comments } from './lib/comments';
import { getContactDetails } from '~/logic/lib/util';


export const LinkDetail = (props) => {
  console.log(props);
  if (!props.node) {
    // TODO: something
    return (
      <div>
        Not found
      </div>
    );
  }
  const { nickname } = getContactDetails(props.contacts[ship]);
  const our = getContactDetails(props.contacts[window.ship]);
  const resourcePath = `${props.ship}/${props.name}`;
  const title = props.resource.metadata.title || resourcePath;

  return (
    <div className="h-100 w-100 overflow-hidden flex flex-column">
      <div
        className={
          'pl4 pt2 flex relative overflow-x-scroll ' +
          'overflow-x-auto-l overflow-x-auto-xl flex-shrink-0 ' +
          'bb bn-m bn-l bn-xl b--gray4'
        }
        style={{ height: 48 }}>
        <SidebarSwitcher
          sidebarShown={props.sidebarShown}
          popout={props.popout}
          api={props.api}
        />
        <Link
          className="dib f9 fw4 pt2 gray2 lh-solid"
          to="/~link">
            {`<- ${title}`}
        </Link>
        <TabBar
          location={props.location}
          popout={props.popout}
          popoutHref={"/~link"}
          settings={"/~link"}
        />
      </div>
      <div className="w-100 mt2 flex justify-center overflow-y-scroll ph4 pb4">
        <div className="w-100 mw7">
          <LinkPreview
            resourcePath={resourcePath}
            post={props.node.post}
            nickname={nickname}
            hideNicknames={props.hideNicknames}
            commentNumber={props.node.children.size} />
          <div className="flex">
            <LinkSubmit
              name={props.name}
              ship={props.ship}
              api={props.api} />
          </div>
          <Comments
            comments={props.node.children}
            resourcePath={resourcePath}
            contacts={props.contacts}
            popout={props.popout}
            api={props.api}
            hideAvatars={props.hideAvatars}
            hideNicknames={props.hideNicknames} />
        </div>
      </div>
    </div>
  );
}

