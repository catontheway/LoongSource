--1006504 �޼�Ӫ����������֮�ȣ�����ս�����ٻ�
--{1006536 �޼�Ӫū��}
function c1006504_On_Enter_Combat(MapID, InstanceID, TargetID)
	-- �������ս������
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
	--15%���ʺ���
		cre.MonsterSay(MapID, InstanceID, TargetID, 30070)
       end
       local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 1006536, x, y, z+5)
	
	
end

aux.RegisterCreatureEvent(1006504,2,"c1006504_On_Enter_Combat")
