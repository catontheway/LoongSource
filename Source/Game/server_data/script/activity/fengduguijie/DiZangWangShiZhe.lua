
--�ز���ʹ�߶Ի��ű�
function n3021003_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if TalkIndex == -1 then
	    local HaveQuest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 20161)
		if HaveQuest == true then
		    -- "�ز���ʹ�ߣ�\\n    ���Ѿ���ȡ����������ɺ�������"
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20, 717)
		    msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		    msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		    msg.AddMsgEvent(MsgID, 1, 706)   --��������
		    msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
	        -- "�ز���ʹ�ߣ�\\n    ���̲������أ������������زء�\\n    �����ز����������º�Ը�������Ⱦ�����֤���ᡣ�������գ��Ĳ��ɷ�������������ӵ��<I>�Ȼ��</I>�������Ҵ���ȡ�������������������ز������ɻ��������꣬�Ե�����ҵ��Բ�����¡�"
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20, 701)
		    msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		    msg.AddMsgEvent(MsgID, 21, 4) -- ����ѡ��
		    msg.AddMsgEvent(MsgID, 1, 702) -- "��������"
		    msg.AddMsgEvent(MsgID, 21, 5)   --����ѡ��
		    msg.AddMsgEvent(MsgID, 1, 703)   --����������[ͬʱ�����¹�ʯ����]��
		    msg.AddMsgEvent(MsgID, 21, 6)   --����ѡ��
		    msg.AddMsgEvent(MsgID, 1, 704)   --����������[ͬʱ����ͨڤ��ħʯ]��
		    msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		    msg.AddMsgEvent(MsgID, 1, 706)   --��������
		    msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	--�����ȡ��ͨ����
	elseif TalkIndex == 4 then
	    local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level < 55 then
	        --��ʾ�������ĵȼ�����55���������ȡʧ�ܡ���
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20, 705)
		    msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		    msg.AddMsgEvent(MsgID, 1, 706)   --��������
		    msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	    else
	        --�ز���ʹ�ߣ�\\n    ��Ҫ�������껹��Ϊ��ȥ�񡢻��ܹ��²źá�������´��ҡ�������ħ���У���֪�����Ƿ�Ը��ǰ����ɱ�ַ����Ե�����ҵ���ն�������
	        local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20, 707)
		    msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		    msg.AddMsgEvent(MsgID, 21, 7)   --ȷ����ť
		    msg.AddMsgEvent(MsgID, 1, 708)   --����ȡ����
		    msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		    msg.AddMsgEvent(MsgID, 1, 706)   --��������
		    msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	--�����ȡ�¹�ʯ����
	elseif TalkIndex == 5 then
	    local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level < 55 then
	        --��ʾ�������ĵȼ�����55���������ȡʧ�ܡ���
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20, 705)
		    msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		    msg.AddMsgEvent(MsgID, 1, 706)   --��������
		    msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	    else
	        --�ز���ʹ�ߣ�\\n    ��Ҫ�������껹��Ϊ��ȥ�񡢻��ܹ��²źá������ˮһ�ص�����ħ���У�����������ɱ�ַ�����ؿɵ�����ҵ���ն������������¹�ʯ�������ģ����ܹ���Բ�����������֣���֪�����Ƿ�Ը�⣿
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20, 709)
		    msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		    msg.AddMsgEvent(MsgID, 21, 8)   --ȷ����ť
		    msg.AddMsgEvent(MsgID, 1, 710)   --����ȡ����[ͬʱ�����¹�ʯ����]��
		    msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		    msg.AddMsgEvent(MsgID, 1, 706)   --��������
		    msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	--�����ȡͨڤ��ħʯ������ʾ�Ի�
	elseif TalkIndex == 6 then
	    local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level < 55 then
	        --��ʾ�������ĵȼ�����55���������ȡʧ�ܡ���
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20, 705)
		    msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		    msg.AddMsgEvent(MsgID, 1, 706)   --��������
		    msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	    else
            --�ز���ʹ�ߣ�\\n    ��Ҫ�������껹��Ϊ��ȥ�񡢻��ܹ��²źá������ˮһ�ص�����ħ���У�����������ɱ�ַ�����ؿɵ�����ҵ���ն�����������ͨڤ��ħʯ���ģ����ܹ���Բ�����𻯵��ɣ���֪�����Ƿ�Ը�⣿
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20, 711)
		    msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		    msg.AddMsgEvent(MsgID, 21, 9)   --ȷ����ť
		    msg.AddMsgEvent(MsgID, 1, 712)   --����ȡ����[ͬʱ����ͨڤ��ħʯ]��
		    msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		    msg.AddMsgEvent(MsgID, 1, 706)   --��������
		    msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	--��ȡ��ͨ����
	elseif TalkIndex == 7 then
	    local DHF = role.GetRoleItemNum(RoleID, 2615902)
		if DHF > 0 then
		    if 0 == role.RemoveFromRole(MapID, InstanceID, RoleID, 2615902, 1, 1000) then
			    role.SetRoleScriptData(RoleID, 1, RoleDataType["FDGJ_QuestType"], 1)   --������ͨ��������
				role.AddQuest(RoleID, 20161)
				--����ȡ�ˡ��������ꡱ����
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 713)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    --��ʾ������û�жȻ�����޷���ȡ�����񡣡�
		        local MsgID = msg.BeginMsgEvent()
		        msg.AddMsgEvent(MsgID, 20, 714)
		        msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			    msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		        msg.AddMsgEvent(MsgID, 1, 706)   --��������
		        msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
		else
		    --��ʾ������û�жȻ�����޷���ȡ�����񡣡�
		        local MsgID = msg.BeginMsgEvent()
		        msg.AddMsgEvent(MsgID, 20, 714)
		        msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			    msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		        msg.AddMsgEvent(MsgID, 1, 706)   --��������
		        msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	--��ȡ�еȾ�������
    elseif TalkIndex == 8 then
	    local DHF = role.GetRoleItemNum(RoleID, 2615902)
		local YGSJH = role.GetRoleItemNum(RoleID, 3200704)
		if YGSJH > 0 then
		    if DHF > 0 then
		        if 0 == role.RemoveFromRole(MapID, InstanceID, RoleID, 3200704, 1, 1000) then
		            if 0 == role.RemoveFromRole(MapID, InstanceID, RoleID, 2615902, 1, 1000) then
					    role.SetRoleScriptData(RoleID, 1, RoleDataType["FDGJ_QuestType"], 2)  --�����еȾ�������
				        role.AddQuest(RoleID, 20161)
						--����ȡ�ˡ��������ꡱ����
						local MsgID = msg.BeginMsgEvent()
		                msg.AddMsgEvent(MsgID, 26, 713)
		                msg.DispatchRoleMsgEvent(RoleID, MsgID)
			        else
						--��ʾ������û�жȻ�����޷���ȡ�����񡣡�
		                local MsgID = msg.BeginMsgEvent()
		                msg.AddMsgEvent(MsgID, 20, 714)
		                msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			            msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		                msg.AddMsgEvent(MsgID, 1, 706)   --��������
		                msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			        end
				else
				    --��ʾ������ȱ���¹�ʯ�������޷�ѡ������
		                local MsgID = msg.BeginMsgEvent()
		                msg.AddMsgEvent(MsgID, 20, 715)
		                msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			            msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		                msg.AddMsgEvent(MsgID, 1, 706)   --��������
		                msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				end
		    else
		        --��ʾ������û�жȻ�����޷���ȡ�����񡣡�
			    local MsgID = msg.BeginMsgEvent()
			    msg.AddMsgEvent(MsgID, 20, 714)
			    msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			    msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
			    msg.AddMsgEvent(MsgID, 1, 706)   --��������
			    msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		    end
		else
			--��ʾ������ȱ���¹�ʯ�������޷�ѡ������
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 715)
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 706)   --��������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	--��ȡ�ߵȾ�������
    elseif TalkIndex == 9 then
	    local DHF = role.GetRoleItemNum(RoleID, 2615902)
		local TMJMS = role.GetRoleItemNum(RoleID, 3200602)
		if TMJMS > 0 then
		    if DHF > 0 then
		        if 0 == role.RemoveFromRole(MapID, InstanceID, RoleID, 3200602, 1, 1000) then
		            if 0 == role.RemoveFromRole(MapID, InstanceID, RoleID, 2615902, 1, 1000) then
					    role.SetRoleScriptData(RoleID, 1, RoleDataType["FDGJ_QuestType"], 3)  --���øߵȾ�������
				        role.AddQuest(RoleID, 20161)
						--����ȡ�ˡ��������ꡱ����
						local MsgID = msg.BeginMsgEvent()
		                msg.AddMsgEvent(MsgID, 26, 713)
		                msg.DispatchRoleMsgEvent(RoleID, MsgID)
			        else
						--��ʾ������û�жȻ�����޷���ȡ�����񡣡�
		                local MsgID = msg.BeginMsgEvent()
		                msg.AddMsgEvent(MsgID, 20, 714)
		                msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			            msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		                msg.AddMsgEvent(MsgID, 1, 706)   --��������
		                msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			        end
				else
				    --��ʾ������ȱ��ͨڤ��ħʯ���޷�ѡ������
		                local MsgID = msg.BeginMsgEvent()
		                msg.AddMsgEvent(MsgID, 20, 716)
		                msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			            msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		                msg.AddMsgEvent(MsgID, 1, 706)   --��������
		                msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				end
		    else
		        --��ʾ������û�жȻ�����޷���ȡ�����񡣡�
			    local MsgID = msg.BeginMsgEvent()
			    msg.AddMsgEvent(MsgID, 20, 714)
			    msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			    msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
			    msg.AddMsgEvent(MsgID, 1, 706)   --��������
			    msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		    end
		else
			--��ʾ������ȱ��ͨڤ��ħʯ���޷�ѡ������
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 716)
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 706)   --��������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
    end
end

aux.RegisterCreatureEvent(3021003, 7, "n3021003_OnTalk")


